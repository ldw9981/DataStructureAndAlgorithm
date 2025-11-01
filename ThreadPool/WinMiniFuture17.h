// WinMiniFuture17.h  (C++17, WinAPI Event 기반, void/T 단일 구현)
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <optional>
#include <variant>   // std::monostate
#include <utility>

// T가 void면 내부 저장은 monostate로 치환
template<class T>
using ValueT = std::conditional_t<std::is_void_v<T>, std::monostate, T>;

// 공유 상태
template<class T>
struct WinState {
	HANDLE ev = CreateEvent(nullptr, TRUE, FALSE, nullptr); // manual-reset
	std::optional<ValueT<T>> val;
	std::exception_ptr ex;

	WinState() { if (!ev) throw std::runtime_error("CreateEvent failed"); }
	~WinState() { if (ev) CloseHandle(ev); }
	WinState(const WinState&) = delete;
	WinState& operator=(const WinState&) = delete;
};

template<class T> class WinFuture;

template<class T>
class WinPromise {
public:
	WinPromise() = default;
	explicit WinPromise(std::shared_ptr<WinState<T>> st) : _st(std::move(st)) {}
	bool valid() const { return static_cast<bool>(_st); }
	HANDLE native_event() const { return _st ? _st->ev : nullptr; }

	// 값 설정( void면 인자 무시, 값 타입이면 인자로 저장 )
	template<class... U>
	void set_value(U&&... u) {
		if constexpr (std::is_void_v<T>) {
			_st->val = ValueT<T>{};     // monostate
		}
		else {
			_st->val.emplace(std::forward<U>(u)...);
		}
		::SetEvent(_st->ev);
	}

	void set_exception(std::exception_ptr e) {
		_st->ex = std::move(e);
		::SetEvent(_st->ev);
	}

private:
	std::shared_ptr<WinState<T>> _st;
	template<class U> friend class WinFuture;
};

template<class T>
class WinFuture {
public:
	WinFuture() = default;
	explicit WinFuture(std::shared_ptr<WinState<T>> st) : _st(std::move(st)) {}
	bool valid() const { return static_cast<bool>(_st); }
	HANDLE native_event() const { return _st ? _st->ev : nullptr; }

	void wait() const {
		if (!_st) throw std::logic_error("invalid WinFuture");
		::WaitForSingleObject(_st->ev, INFINITE);
	}
	bool wait_for(DWORD ms) const {
		if (!_st) throw std::logic_error("invalid WinFuture");
		return ::WaitForSingleObject(_st->ev, ms) == WAIT_OBJECT_0;
	}

	// void면 반환 없음, 값 타입이면 이동 반환
	auto get() {
		wait();
		if (_st->ex) std::rethrow_exception(_st->ex);
		if constexpr (std::is_void_v<T>) {
			return; // 아무 것도 반환하지 않음
		}
		else {
			auto out = std::move(*_st->val);
			_st->val.reset();
			return out;
		}
	}

private:
	std::shared_ptr<WinState<T>> _st;
};

// Promise/Future 쌍 생성
template<class T>
inline std::pair<WinPromise<T>, WinFuture<T>> make_win_promise_future() {
	auto st = std::make_shared<WinState<T>>();
	return { WinPromise<T>(st), WinFuture<T>(st) };
}
