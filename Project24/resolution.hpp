#pragma once

namespace cmp_resolution
{
	using ratio = uint8_t;
	using resolution = std::pair<pixels, pixels>;

	template <pixels Rx, pixels Ry, ratio X, ratio Y>
	constexpr bool resolution_has_ratio()
	{
		return double(Rx) / double(Ry) ==
			double(X) / double(Y);
	}

	template <uint8_t ReducePercent>
	resolution percent_reduce(resolution&& r) 
	{
		static_assert(ReducePercent <= 100 && ReducePercent != 0, "percent is too high");
		r.first = r.first * (100 - ReducePercent) / 100;
		r.second = r.second * (100 - ReducePercent) / 100;
		return r;
	}

	template <uint8_t ReducePercent, pixels V>
	constexpr pixels percent_reduce()
	{
		return V * (100 - ReducePercent) / 100;
	}

	template <ratio X, ratio Y, pixels ResolutionX, pixels ResolutionY>
	class cell_map_resolution
	{
		static_assert(resolution_has_ratio<ResolutionX, ResolutionY, X, Y>(),
			"wrong resolution value for current aspect ratio");
	public:
		static resolution get()
		{
			return percent_reduce<10>
			(
				{
					ResolutionX * 12 / X,
					ResolutionY * 9 / Y
				}
			);
		}

		static constexpr pixels get_x() { return percent_reduce<10, ResolutionX * 12 / X>(); }
		static constexpr pixels get_y() { return percent_reduce<10, ResolutionY * 9 / Y>(); }
	};

	template <pixels ResolutionX, pixels ResolutionY>
	class cell_map_resolution<16, 9, ResolutionX, ResolutionY>
	{
		static_assert(resolution_has_ratio<ResolutionX, ResolutionY, 16, 9>(),
			"wrong resolution value for current aspect ratio");
	public:
		static resolution get()
		{
			return percent_reduce<10>
			(
				{ 
					ResolutionX * 3 / 4,
					ResolutionY 
				}
			);
		}

		static constexpr pixels get_x() {	return percent_reduce<10, ResolutionX * 3 / 4>(); }
		static constexpr pixels get_y() {	return percent_reduce<10, ResolutionY>(); }
		static constexpr resolution get_screen_res() { return { ResolutionX, ResolutionY }; }
	};

	template <pixels ResolutionX, pixels ResolutionY>
	class cell_map_resolution<16, 10, ResolutionX, ResolutionY>
	{
		static_assert(resolution_has_ratio<ResolutionX, ResolutionY, 16, 10>(),
			"wrong resolution value for current aspect ratio");
	public:
		static resolution get()
		{
			return percent_reduce<10>
			(
				{
					ResolutionX * 3 / 4,
					ResolutionY * 9 / 10
				}
			);
		}

		static constexpr pixels get_x() { return percent_reduce<10, ResolutionX * 3 / 4>(); }
		static constexpr pixels get_y() { return percent_reduce<10, ResolutionY * 9 / 10>(); }
		static constexpr resolution get_screen_res() { return { ResolutionX, ResolutionY }; }
	};

	template <pixels ResolutionX, pixels ResolutionY>
	class cell_map_resolution<4, 3, ResolutionX, ResolutionY>
	{
		static_assert(resolution_has_ratio<ResolutionX, ResolutionY, 4, 3>(),
			"wrong resolution value for current aspect ratio");
	public:
		static resolution get()
		{
			return percent_reduce<10>
			(
				{
					ResolutionX,
					ResolutionY 
				}
			);
		}

		static constexpr pixels get_x() { return percent_reduce<10, ResolutionX>(); }
		static constexpr pixels get_y() { return percent_reduce<10, ResolutionY>(); }
		static constexpr resolution get_screen_res() { return { ResolutionX, ResolutionY }; }
	};
}