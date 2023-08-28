package cn.abellee.cniface.platform.util;

import java.time.*;
import java.time.format.DateTimeFormatter;

/**
 * @author abel
 * @date 2022/6/5 5:52 PM
 */
public class TimeUtil {

    public static final String DEFAULT_ZONE = "+8";

    public static final String DEFAULT_DATE_FORMAT = "yyyy-MM-dd";

    public static long now() {
        return getCurrentLocalDateTime().atZone(ZoneId.of(DEFAULT_ZONE)).toInstant().toEpochMilli();
    }

    public static int localDateTimeToInt(LocalDateTime localDateTime) {
        return (int)localDateTime.toEpochSecond(ZoneOffset.of(DEFAULT_ZONE));
    }

    public static long localDateTimeToLong(LocalDateTime localDateTime) {
        return localDateTime.atZone(ZoneId.of(DEFAULT_ZONE)).toInstant().toEpochMilli();
    }

    public static LocalDateTime getCurrentLocalDateTime() {
        return LocalDateTime.now();
    }

    public static LocalTime getCurrentLocalTime() {
        return LocalTime.now();
    }

    public static LocalDate getCurrentLocalDate() {
        return LocalDate.now();
    }


    public static String intToDateTime(int val, String patter) {
        DateTimeFormatter df = DateTimeFormatter.ofPattern(patter);
        return df.format(LocalDateTime.ofInstant(Instant.ofEpochSecond(val), ZoneOffset.of(DEFAULT_ZONE)));
    }

    public static boolean isToday(int timestamp) {
        final LocalDateTime localDateTime = LocalDateTime.ofInstant(Instant.ofEpochSecond(timestamp), ZoneOffset.of(DEFAULT_ZONE));
        return localDateTime.toLocalDate().isEqual(getCurrentLocalDate());
    }

    public static long addDay(long timestamp, long days) {
        return timestamp + (days * 86400 * 1000);
    }

    public static long startTodayLong() {
        return localDateTimeToLong(getCurrentLocalDateTime().withHour(0)
                .withMinute(0)
                .withSecond(0)
                .withNano(0));
    }

    public static LocalDateTime startToday() {
        return getCurrentLocalDateTime().withHour(0)
                .withMinute(0)
                .withSecond(0)
                .withNano(0);
    }

    public static Integer endToday() {
        return localDateTimeToInt(getCurrentLocalDateTime().withHour(23)
                .withMinute(59)
                .withSecond(59)
                .withNano(999999999));
    }

    public static Integer endToday(int timestamp) {
        return localDateTimeToInt(getCurrentLocalDateTime().withHour(23)
                .withMinute(59)
                .withSecond(59)
                .withNano(999999999));
    }

    public static String convertTimeToDefaultDateString(long time) {
        return convertTimeToString(time, DEFAULT_DATE_FORMAT);
    }

    public static String convertTimeToString(long time, String format){
        DateTimeFormatter ftf = DateTimeFormatter.ofPattern(format);
        return ftf.format(LocalDateTime.ofInstant(Instant.ofEpochMilli(time), ZoneId.of(DEFAULT_ZONE)));
    }

    public static long convertDateStringToLong(String timeStr, String format){
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern(format);
        return localDateTimeToLong(LocalDateTime.from(LocalDate.parse(timeStr, formatter).atStartOfDay()));
    }
}

