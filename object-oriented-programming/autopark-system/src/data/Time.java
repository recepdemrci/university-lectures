package data;

import data.Exceptions.InvalidTimeException;

import java.util.Calendar;

public class Time {
    private int hour, minute;

    // Constructors
    public Time(int hour, int minute) throws InvalidTimeException {
            if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
                throw new InvalidTimeException("Time is invalid.");
            }
        this.hour = hour;
        this.minute = minute;
    }

    // Methods
    public static Time getTime() {
        Calendar calendar = Calendar.getInstance();
        try {
            Time now = new Time( calendar.get(Calendar.HOUR_OF_DAY),
            calendar.get(Calendar.MINUTE));
            return now;
        }catch(InvalidTimeException t){t.printStackTrace();}

        return null;
    }

    public int getDifference(Time other) {
        int hour = other.hour - this.hour;
        int minute = other.minute - this.minute;

        return ( (hour*60) + minute );
    }

    public boolean isAfterThan(Time other) {
        if ( this.hour > other.hour ){return true;}
        else if(this.hour < other.hour){return false;}
        else {
            if ( this.minute > other.minute ){return true;}
            else { return false; }
        }
    }

    @Override
    public String toString() {
        return "(" + hour + ":" + minute + ')';
    }

    // Getter Setter
    public int getHour() {
        return hour;
    }

    public void setHour(int hour) {
        this.hour = hour;
    }

    public int getMinute() {
        return minute;
    }

    public void setMinute(int minute) {
        this.minute = minute;
    }
}
