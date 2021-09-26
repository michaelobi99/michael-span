#pragma once
#include <vector>
#include <deque>
#include <list>
#include <array>
#include <type_traits>
#include <concepts>
#include <algorithm>
#include <string>
namespace michael {
	template <typename T, std::size_t __size = 0>
	class span {
	public:
		//deleting constructors accepting list and deques....cos i dont know how to do it with concepts...yet
		//or if it is even possible....but it has to be(this is c++ afterall)
		span(std::deque<std::remove_const_t<T>>& Container) = delete;
		span(std::list<std::remove_const_t<T>> const& Container) = delete;
		span(std::list<std::remove_const_t<T>>& Container) = delete;
		span(std::deque<std::remove_const_t<T>> const& Container) = delete;

		//default constructor
		constexpr span() = default;

		//constructor with parameters
		template<std::size_t S>
		constexpr span(T(&__containerPtr)[S]) : arrayPtr(__containerPtr), arraySize(S) {}

		template <typename __container__>
		constexpr span(__container__& Container) : arrayPtr(Container.data()), arraySize(Container.size()) {}

		constexpr span(T* __containerPtr, std::size_t extent) : 
			arrayPtr(__containerPtr), arraySize(extent) {}

		//copy constructor
		constexpr span(span const& other) {
			arrayPtr = nullptr;
			this->arrayPtr = other.arrayPtr;
			this->arraySize = other.arraySize;
		}
		//copy assignment operator
		constexpr span operator=(span const& other) {
			if (!(*this == other)) {
				this->arrayPtr = other.arrayPtr;
				this->arraySize = other.arraySize;
			}
			return *this;
		}

		constexpr span(span&& other) = default;//default move constructor

		constexpr bool operator==(span const& other) {
			return this->arrayPtr == other.arrayPtr;
		}
		span subspan(int start, int extent){
			T* start_ptr =  this->arrayPtr + start ;
			return span(start_ptr, extent);
		}
		[[nodiscard]] T operator[] (std::size_t const index) {
			if (index < 0 || index >= arraySize)
				throw std::out_of_range((std::to_string(index) + "is out of range"));
			else
				return *(arrayPtr + index);
		}
		
		[[nodiscard]] T* data() const noexcept {
			return arrayPtr;
		}

		[[nodiscard]] bool empty() const noexcept {
			return arraySize > 0 ? false : true;
		}

		[[nodiscard]] std::size_t size() const {
			return arraySize;
		}

		[[nodiscard]] T front() const {
			if (arraySize <= 0)
				throw std::out_of_range("ERROR: out of range");
			else
				return *(arrayPtr);
		}

		[[nodiscard]] T back() const {
			if (arraySize <= 0)
				throw std::out_of_range("ERROR: out of range");
			else
				return *(arrayPtr + arraySize - 1);
		}
	private:
		T* arrayPtr{ nullptr };
		std::size_t arraySize{ __size };
	};
	template <typename T, std::size_t S>
	span(T(&)[S])->span<T, S>;

	template <typename T, std::size_t S>
	span(T const(&)[S])->span<T const, S>;

	template <typename T, size_t S>
	span(std::vector<T>&)->span<T>;

	template <typename T, size_t S>
	span(std::vector<T> const&)->span<T const>;

	template <typename T, size_t S>
	span(std::array<T, S>&)->span<T>;

	template <typename T, size_t S>
	span(std::array<T, S> const&)->span<T const>;


	//iterators to the span class
	template <typename T>
	class span_iterator_type {
	public:
		span_iterator_type() = default;
		span_iterator_type(T* element) : currentElement{ element } {}
		bool operator!=(span_iterator_type<T> const& other_span_iterator) {
			return this->currentElement != other_span_iterator.currentElement;
		}
		bool operator==(span_iterator_type<T> const& other_span_iterator) {
			return !(this->currentElement != other_span_iterator.currentElement);
		}
		span_iterator_type<T>& operator++() {
			++currentElement;
			return *this;
		}
		span_iterator_type<T>& operator++(int) {
			span_iterator_type<T> temp_iterator = *this;
			++*this;
			return temp_iterator;
		}
		T& operator*() {
			return *currentElement;
		}
		T const& operator*() const{
			return *currentElement;
		}
	private:
		T* currentElement{ nullptr };
	};

	//Alias templates for mutable and constant iterators
	template <typename T>
	using span_iterator = span_iterator_type<T>;

	template <typename T>
	using const_span_iterator = span_iterator_type<T const>;


	//free begin and end functions that returns the corresponding begin and end iterators
	template <typename T, std::size_t S>
	inline span_iterator<T> begin(span<T, S>& span) {
		return span_iterator<T>(span.data());
	}
	template <typename T, std::size_t S>
	inline const_span_iterator<T> begin(span<T, S> const& span) {
		return const_span_iterator<T>(span.data());
	}
	template <typename T, std::size_t S>
	inline span_iterator<T> end(span<T, S>& span) {
		return span_iterator<T>(span.data() + span.size());
	}
	template <typename T, std::size_t S>
	inline const_span_iterator<T> end(span<T, S> const& span) {
		return const_span_iterator<T>(span.data() + span.size());
	}
}