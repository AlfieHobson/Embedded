struct Time RTC_getTime(void);
void RTC_setTime(struct Time);
bool RTC_isDay(struct Time time);
void RTC_setTimeBackup(void);