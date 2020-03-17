#ifndef XNUMER_UTILITY_FUNCTIONS
#define XNUMER_UTILITY_FUNCTIONS

namespace utility {
	template<typename T>
	void swap(T& a, T& b) {
		T tmp = a;
		a = b;
		b = tmp;
	}

	template<typename Iter>
	class range_based_wrapper {
	public:
		range_based_wrapper(Iter from, Iter to) : from(from), to(to) {}
		Iter begin() {
			return from;
		}
		Iter end() {
			return to;
		}
	private:
		Iter from, to;
	};

	template<typename T>
	using isSigned = std::enable_if_t<std::is_signed_v<T>, bool>;

	template<typename T>
	using isUnsigned = std::enable_if_t<!std::is_signed_v<T>, bool>;

	template<typename T, typename = void>
	struct is_decrementable : std::false_type {};

	template<typename T>
	struct is_decrementable<T, std::void_t<decltype(--std::declval<T>())>> : std::true_type {};

	template<typename T>
	using isDecrementable = std::enable_if_t<is_decrementable<T>::value, bool>;

	template<typename T, typename isUnsigned<T> = true, typename isDecrementable<T> = true>
	struct unsignedDecrement {
	public:
		T& operator()(T& value) {
			return (value == static_cast<T>(0)) ? value : --value;
		}
	};
}

#endif