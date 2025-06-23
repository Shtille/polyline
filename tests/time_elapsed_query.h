#pragma once

#include <cstdint>

namespace poly {

/**
 * @brief      This class describes a time elapsed query.
 */
class TimeElapsedQuery
{
public:
	/**
	 * @brief      Constructs a new instance.
	 */
	TimeElapsedQuery();

	/**
	 * @brief      Destroys the object.
	 */
	~TimeElapsedQuery();

	/**
	 * @brief      Begins query
	 */
	void Begin();

	/**
	 * @brief      Ends query
	 */
	void End();

	/**
	 * @brief      Determines if result available.
	 *
	 * @return     True if result available, False otherwise.
	 */
	bool IsResultAvailable();

	/**
	 * @brief      Gets the result.
	 *
	 * @param[in]  sync  Whether call is synchronous
	 *
	 * @return     True if data is ready and false otherwise.
	 */
	bool GetResult(bool sync);

	/**
	 * @brief      Gets the elapsed time.
	 *
	 * @return     The elapsed time in nanoseconds.
	 */
	uint64_t GetElapsedTime() const;

private:
	uint64_t elapsed_time_; // elapsed time in nanoseconds
	uint32_t id_;
};

} // namespace poly