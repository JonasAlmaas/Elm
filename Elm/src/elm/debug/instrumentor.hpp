#pragma once

#include "elm/core/base.hpp"
#include <chrono>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>

namespace elm {

	struct profile_result {
		std::string name;

		std::chrono::duration<double, std::micro> start_us;
		std::chrono::microseconds elapsed_time_us;
		std::thread::id thread_id;
	};

	struct instrumentation_session {
		std::string name;
	};

	struct instrumentor
	{
		instrumentor(const instrumentor &) = delete;
		instrumentor(instrumentor &&) = delete;

		void begin_session(const std::string &name, const std::string &fpath = "resuls.json");
		void end_session(void);

		void write_profile(const profile_result &result);

		static instrumentor *get() {
			static instrumentor instance;
			return &instance;
		}
	private:
		instrumentor(void): current_session(nullptr) {}
		~instrumentor(void)
		{
			end_session();
		}

		void write_header(void);
		void write_footer(void);

		// Note: You must already own lock on mutex before
		// calling internal_end_session()
		void internal_end_session(void);

	private:
		std::mutex mutex;
		instrumentation_session *current_session;
		std::ofstream output_stream;
	};

	struct instrumentation_timer
	{
		instrumentation_timer(const char *name):
			name(name),
			start_timepoint(std::chrono::steady_clock::now()), stopped(false)
		{
		}

		~instrumentation_timer(void)
		{
			if (!this->stopped) {
				stop();
			}
		}

		void stop();

	private:
		const char *name;
		std::chrono::time_point<std::chrono::steady_clock> start_timepoint;
		bool stopped;
	};

	namespace instrumentor_utils {

		template <size_t N>
		struct change_result {
			char data[N];
		};

		template <size_t N, size_t K>
		constexpr auto cleanup_output_string(const char(&expr)[N], const char(&remove)[K])
		{
			change_result<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N) {
				size_t matchIndex = 0;
				while (matchIndex < K - 1
						&& srcIndex + matchIndex < N - 1
						&& expr[srcIndex + matchIndex] == remove[matchIndex]) {
					++matchIndex;
				}
				if (matchIndex == K - 1) {
					srcIndex += matchIndex;
				}
				result.data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				++srcIndex;
			}
			return result;
		}
	}
}

#if ELM_ENABLE_PROFILING
	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define ELM_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define ELM_FUNC_SIG __PRETTY_FUNCTION__
	#elif (defined(__FUNCSIG__) || (_MSC_VER))
		#define ELM_FUNC_SIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define ELM_FUNC_SIG __FUNCTION__
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define ELM_FUNC_SIG __FUNC__
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define ELM_FUNC_SIG __func__
	#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define ELM_FUNC_SIG __func__
	#else
		#define ELM_FUNC_SIG "ELM_FUNC_SIG unknown!"
	#endif

	#define ELM_PROFILE_BEGIN_SESSION(name, filepath) ::elm::instrumentor::get()->begin_session(name, filepath)
	#define ELM_PROFILE_END_SESSION() ::elm::instrumentor::get()->end_session()
	#define ELM_PROFILE_SCOPE_LINE2(name, line)\
		constexpr auto fixed_name##line = ::elm::instrumentor_utils::cleanup_output_string(name, "__cdecl ");\
		::elm::instrumentation_timer timer##line(fixed_name##line.data)
	#define ELM_PROFILE_SCOPE_LINE(name, line) ELM_PROFILE_SCOPE_LINE2(name, line)
	#define ELM_PROFILE_SCOPE(name) ELM_PROFILE_SCOPE_LINE(name, __LINE__)
	#define ELM_PROFILE_FUNCTION() ELM_PROFILE_SCOPE(ELM_FUNC_SIG)

	#if ELM_ENABLE_PROFILING_RENDERER
		#define ELM_PROFILE_RENDERER_SCOPE(name) ELM_PROFILE_SCOPE_LINE(name, __LINE__)
		#define ELM_PROFILE_RENDERER_FUNCTION() ELM_PROFILE_SCOPE(ELM_FUNC_SIG)
	#else
		#define ELM_PROFILE_RENDERER_SCOPE(name)
		#define ELM_PROFILE_RENDERER_FUNCTION()
	#endif
#else
	#define ELM_PROFILE_BEGIN_SESSION(name, filepath)
	#define ELM_PROFILE_END_SESSION()
	#define ELM_PROFILE_SCOPE(name)
	#define ELM_PROFILE_FUNCTION()

	#define ELM_PROFILE_RENDERER_SCOPE(name)
	#define ELM_PROFILE_RENDERER_FUNCTION()
#endif
