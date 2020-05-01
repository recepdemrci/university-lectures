package data;

import data.Exceptions.InvalidDateException;

import java.util.Calendar;

public class Date {

    private int day, month, year;

    // Constructors
    public Date(int day, int month, int year) throws InvalidDateException {
        if(day < 0 || day > 31 ||  month < 0 ||month > 12 ){
            throw new InvalidDateException("Date is invalid.");
        }

        this.day = day;
        this.month = month;
        this.year = year;
    }

    // Methods
    public static Date getToday() {
        Calendar calendar = Calendar.getInstance();
        try{
            Date today = new Date( calendar.get(Calendar.DATE),
                                   calendar.get(Calendar.MONTH)+1,
                                   calendar.get(Calendar.YEAR)  );
            return today;
        }catch (InvalidDateException d){d.printStackTrace();}

        return null;
    }

    public boolean isAfterThan(Date other) {
        if ( this.year > other.year ){return true;}
        else if(this.year < other.year){return false;}
        else {
            if ( this.month > other.month ){return true;}
            else if(this.month < other.month){return false;}
            else {
                if ( this.day > other.day ){return true;}
                return false;
            }
        }
    }

    public boolean isBeforeThan(Date other) {
        if ( this.year < other.year ){return true;}
        else if(this.year > other.year){return false;}
        else {
            if ( this.month < other.month ){return true;}
            else if(this.month > other.month){return false;}
            else {
                if ( this.day < other.day ){return true;}
                return false;
            }
        }
    }

    public boolean isEqualsWith(Date other) {
        if (this.year == other.year)
            if (this.month == other.month)
                if (this.day == other.day) {
                    return true;
                }
        return false;
    }


    @Override
    public String toString() {
        return "Date[" +
                " " + day +
                "." + month +
                "." + year +
                " ]";
    }
}
