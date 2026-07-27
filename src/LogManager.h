class LogManager {
    public:
        static LogManager &getInstance();
    private:
        //Singleton stuff
        LogManager() {
            
        }

        LogManager(const LogManager&) = delete;
        LogManager& operator=(const LogManager&) = delete;
        
        void process_log();
};