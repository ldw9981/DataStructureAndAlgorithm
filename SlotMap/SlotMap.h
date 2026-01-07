#pragma once
#include <vector>
#include <cstdint>
#include <cassert>

template <typename T>
class SlotMap {
public:
	struct Handle {
		uint32_t slotIndex;    // 외부에 알려지는 슬롯 인덱스
		uint32_t generation;   // 그 슬롯의 세대(버전)
	};

private:
	struct Slot {
		uint32_t denseIndex = 0; // 내부에서 관리하는 실제 데이터의 인덱스  slot → dense
		uint32_t generation = 0; // 세대: stale handle(옛 핸들) 무효화용		
		bool alive = false;      // 현재 살아있는지(삭제 여부)
	};

	std::vector<T> dense;                 // T가 실제로 저장되는 연속 배열 (빠른 순회/캐시 친화)
	std::vector<uint32_t> denseToSlot;    // dense index -> slot index
	std::vector<Slot> slots;              // 발행한 Handle의 SlotIndex만큼 유지한다.
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

		//
		Slot& er_slot = slots[h.slotIndex];
		uint32_t er_denseIndex = er_slot.denseIndex;
		uint32_t lastDense = static_cast<uint32_t>(dense.size() - 1);

		
		if (er_denseIndex != lastDense) {
			// swap dense data
			dense[er_denseIndex] = std::move(dense[lastDense]);

			uint32_t movedSlot = denseToSlot[lastDense];
			denseToSlot[er_denseIndex] = movedSlot;
			slots[movedSlot].denseIndex = er_denseIndex;
		}

		//원소 개수(size)만 1 줄어들고, 보통은 할당된 메모리(capacity)는 그대로다.
		dense.pop_back();
		denseToSlot.pop_back();

		er_slot.alive = false;
		er_slot.generation++;          // ⭐ 핵심: 세대 증가
		freeSlots.push_back(h.slotIndex);

		return true;
	}

	// 접근
	T* get(Handle h) {
		if (!isValid(h)) return nullptr;
		return &dense[slots[h.slotIndex].denseIndex];
	}

	const T* get(Handle h) const {
		if (!isValid(h)) return nullptr;
		return &dense[slots[h.slotIndex].denseIndex];
	}

	bool isValid(Handle h) const {
		// 발급한 index보다 크면 심각한 메모리 오류
		assert(h.slotIndex < slots.size() && "Handle.index is out of SlotMap range (corrupt or foreign handle)");
		if (h.slotIndex >= slots.size()) 
			return false;

		const Slot& slot = slots[h.slotIndex];
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
