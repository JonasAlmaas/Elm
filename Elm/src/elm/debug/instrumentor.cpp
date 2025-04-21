#include "instrumentor.hpp"

namespace elm {

	void instrumentor::begin_session(const std::string &name, const std::string &fpath)
	{
		std::lock_guard lock(this->mutex);
		if (this->current_session) {
			// If there is already a current session, then close it before beginning new one.
			// Subsequent profiling output meant for the original session will end up in the
			// newly opened session instead. That's better than having badly formatted
			// profiling output.
			if (log::get_core_logger()) { // Edge case: BeginSession() might be before log::init()
				ELM_CORE_ERROR("Instrumentor::BeginSession('{0}') when session \"{1}\" already open", name, this->current_session->name);
			}
			internal_end_session();
		}

		this->output_stream.open(fpath);

		if (this->output_stream.is_open()) {
			this->current_session = new instrumentation_session({ name });
			write_header();
		} else {
			if (log::get_core_logger()) { // Edge case: BeginSession() might be before log::init()
				ELM_CORE_ERROR("Instrumentor could not open results file \"{0}\"", fpath);
			}
		}
	}

	void instrumentor::end_session()
	{
		std::lock_guard lock(this->mutex);
		internal_end_session();
	}

	void instrumentor::write_profile(const profile_result &result)
	{
		std::stringstream json;

		json << std::setprecision(3) << std::fixed;
		json << ",{";
		json << "\"cat\":\"function\",";
		json << "\"dur\":" << (result.elapsed_time_us.count()) << ',';
		json << "\"name\":\"" << result.name << "\",";
		json << "\"ph\":\"X\",";
		json << "\"pid\":0,";
		json << "\"tid\":" << result.thread_id << ",";
		json << "\"ts\":" << result.start_us.count();
		json << "}";

		std::lock_guard lock(this->mutex);
		if (this->current_session) {
			this->output_stream << json.str();
			this->output_stream.flush();
		}
	}

	void instrumentor::write_header()
	{
		this->output_stream << "{\"otherData\": {},\"traceEvents\":[{}";
		this->output_stream.flush();
	}

	void instrumentor::write_footer()
	{
		this->output_stream << "]}";
		this->output_stream.flush();
	}

	void instrumentor::internal_end_session()
	{
		if (this->current_session) {
			write_footer();
			this->output_stream.close();
			delete this->current_session;
			this->current_session = nullptr;
		}
	}

	void instrumentation_timer::stop()
	{
		auto end_timepoint = std::chrono::steady_clock::now();
		auto high_res_start = std::chrono::duration<double, std::micro>{ this->start_timepoint.time_since_epoch() };
		auto elapsed_time = std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint).time_since_epoch()
			- std::chrono::time_point_cast<std::chrono::microseconds>(this->start_timepoint).time_since_epoch();

		instrumentor::get()->write_profile({ this->name, high_res_start, elapsed_time, std::this_thread::get_id() });

		this->stopped = true;
	}
}
