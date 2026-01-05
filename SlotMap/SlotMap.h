#pragma once
#include <vector>
#include <cstdint>
#include <cassert>

template <typename T>
class SlotMap {
public:
	struct Handle {
		uint32_t index;
		uint32_t generation;
	};

private:
	struct Slot {
		uint32_t generation = 0;
		uint32_t denseIndex = 0;
		bool alive = false;
	};

	std::vector<T> dense;                 // 연속 데이터
	std::vector<uint32_t> denseToSlot;    // dense index -> slot index
	std::vector<Slot> slots;              // slot table
	std::vector<uint32_t> freeSlots;      // 재사용 슬롯

public:
	// 생성
	Handle insert(const T& value) {
		uint32_t slotIndex;

		if (!freeSlots.empty()) {
			slotIndex = freeSlots.back();
			freeSlots.pop_back();
		}
		else {
			slotIndex = static_cast<uint32_t>(slots.size());
			slots.emplace_back();
		}

		uint32_t denseIndex = static_cast<uint32_t>(dense.size());
		dense.push_back(value);
		denseToSlot.push_back(slotIndex);

		Slot& slot = slots[slotIndex];
		slot.denseIndex = denseIndex;
		slot.alive = true;

		return Handle{ slotIndex, slot.generation };
	}

	// 삭제 (O(1), swap-remove)
	bool erase(Handle h) {
		if (!isValid(h)) return false;

		Slot& slot = slots[h.index];
		uint32_t denseIndex = slot.denseIndex;
		uint32_t lastDense = static_cast<uint32_t>(dense.size() - 1);

		if (denseIndex != lastDense) {
			// swap dense data
			dense[denseIndex] = std::move(dense[lastDense]);

			uint32_t movedSlot = denseToSlot[lastDense];
			denseToSlot[denseIndex] = movedSlot;
			slots[movedSlot].denseIndex = denseIndex;
		}

		dense.pop_back();
		denseToSlot.pop_back();

		slot.alive = false;
		slot.generation++;          // ⭐ 핵심: 세대 증가
		freeSlots.push_back(h.index);

		return true;
	}

	// 접근
	T* get(Handle h) {
		if (!isValid(h)) return nullptr;
		return &dense[slots[h.index].denseIndex];
	}

	const T* get(Handle h) const {
		if (!isValid(h)) return nullptr;
		return &dense[slots[h.index].denseIndex];
	}

	bool isValid(Handle h) const {
		if (h.index >= slots.size()) return false;
		const Slot& slot = slots[h.index];
		return slot.alive && slot.generation == h.generation;
	}

	// iteration용 (핸들 없이 순회)
	std::vector<T>& data() { return dense; }
	const std::vector<T>& data() const { return dense; }

	// Iterator 지원 (range-based for loop 사용 가능)
	typename std::vector<T>::iterator begin() { return dense.begin(); }
	typename std::vector<T>::iterator end() { return dense.end(); }
	typename std::vector<T>::const_iterator begin() const { return dense.begin(); }
	typename std::vector<T>::const_iterator end() const { return dense.end(); }
	typename std::vector<T>::const_iterator cbegin() const { return dense.cbegin(); }
	typename std::vector<T>::const_iterator cend() const { return dense.cend(); }

	size_t size() const { return dense.size(); }
	bool empty() const { return dense.empty(); }
};
