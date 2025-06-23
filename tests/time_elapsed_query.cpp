#include "time_elapsed_query.h"

#include <glad/glad.h>

namespace poly {

TimeElapsedQuery::TimeElapsedQuery()
: elapsed_time_(0)
, id_(0)
{
	glGenQueries(1, &id_);
	// int32_t num_bits;
	// glGetQueryiv(GL_TIME_ELAPSED, GL_QUERY_COUNTER_BITS, &num_bits);
}
TimeElapsedQuery::~TimeElapsedQuery()
{
	glDeleteQueries(1, &id_);
}
void TimeElapsedQuery::Begin()
{
	glBeginQuery(GL_TIME_ELAPSED, id_);
}
void TimeElapsedQuery::End()
{
	glEndQuery(GL_TIME_ELAPSED);
}
bool TimeElapsedQuery::IsResultAvailable()
{
	int32_t available = 0;
	glGetQueryObjectiv(id_, GL_QUERY_RESULT_AVAILABLE, &available);
	return available != 0;
}
bool TimeElapsedQuery::GetResult(bool sync)
{
	if (sync || IsResultAvailable())
	{
		// Function will stall the CPU until result is ready, if result is not available yet
		glGetQueryObjectui64v(id_, GL_QUERY_RESULT, &elapsed_time_);
		return true;
	}
	return false;
}
uint64_t TimeElapsedQuery::GetElapsedTime() const
{
	return elapsed_time_;
}

} // namespace poly