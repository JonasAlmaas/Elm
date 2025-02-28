#pragma once

namespace elm {

	class Application
	{
	public:
		Application(void);
		virtual ~Application(void);

		// To be defined by the client
		static Application *Create(void);

		void run(void);

		static inline Application *get(void) { return s_instance; }

	private:
		bool m_running;

	private:
		static Application *s_instance;
	};
}
