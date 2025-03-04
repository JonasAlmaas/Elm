#include "instrumentor.h"

namespace elm {

	void instrumentor::begin_session(const std::string &name, const std::string &fpath)
	{
		std::lock_guard lock(m_mutex);
		if (m_current_session) {
			// If there is already a current session, then close it before beginning new one.
			// Subsequent profiling output meant for the original session will end up in the
			// newly opened session instead. That's better than having badly formatted
			// profiling output.
			if (log::get_core_logger()) { // Edge case: BeginSession() might be before log::init()
				ELM_CORE_ERROR("Instrumentor::BeginSession('{0}') when session \"{1}\" already open", name, m_current_session->name);
			}
			internal_end_session();
		}

		m_output_stream.open(fpath);

		if (m_output_stream.is_open()) {
			m_current_session = new instrumentation_session({ name });
			write_header();
		} else {
			if (log::get_core_logger()) { // Edge case: BeginSession() might be before log::init()
				ELM_CORE_ERROR("Instrumentor could not open results file \"{0}\"", fpath);
			}
		}
	}

	void instrumentor::end_session(void)
	{
		std::lock_guard lock(m_mutex);
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

		std::lock_guard lock(m_mutex);
		if (m_current_session) {
			m_output_stream << json.str();
			m_output_stream.flush();
		}
	}

	instrumentor::instrumentor(void)
		: m_current_session(nullptr)
	{
	}

	instrumentor::~instrumentor(void)
	{
		end_session();
	}

	void instrumentor::write_header(void)
	{
		m_output_stream << "{\"otherData\": {},\"traceEvents\":[{}";
		m_output_stream.flush();
	}

	void instrumentor::write_footer(void)
	{
		m_output_stream << "]}";
		m_output_stream.flush();
	}

	void instrumentor::internal_end_session(void)
	{
		if (m_current_session) {
			write_footer();
			m_output_stream.close();
			delete m_current_session;
			m_current_session = nullptr;
		}
	}

	instrumentation_timer::instrumentation_timer(const char *name)
		: m_name(name), m_start_timepoint(std::chrono::steady_clock::now()), m_stopped(false)
	{
	}

	instrumentation_timer::~instrumentation_timer()
	{
		if (!m_stopped) {
			stop();
		}
	}

	void instrumentation_timer::stop()
	{
		auto end_timepoint = std::chrono::steady_clock::now();
		auto high_res_start = std::chrono::duration<double, std::micro>{ m_start_timepoint.time_since_epoch() };
		auto elapsed_time = std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint).time_since_epoch()
			- std::chrono::time_point_cast<std::chrono::microseconds>(m_start_timepoint).time_since_epoch();

		instrumentor::get()->write_profile({ m_name, high_res_start, elapsed_time, std::this_thread::get_id() });

		m_stopped = true;
	}
}
