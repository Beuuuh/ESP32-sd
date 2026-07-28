class LogManager {
    public:
        static LogManager &getInstance();
    private:
        //Singleton stuff
        LogManager() {
            init();
        }

        LogManager(const LogManager&) = delete;
        LogManager& operator=(const LogManager&) = delete;
        
        void process_log(const char* format, va_list args);
        void init();
};