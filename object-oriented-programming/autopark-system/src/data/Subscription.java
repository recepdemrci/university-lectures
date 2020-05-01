package data;

import data.Exceptions.SubscriptionTimeException;

public class Subscription {

    private Date begin, end;
    private SubscribedVehicle vehicle;


    // Constructor
    public Subscription(Date begin, Date end, String plate) throws SubscriptionTimeException {
        if(end.isBeforeThan(begin)){
            throw new SubscriptionTimeException("Subscription time is invalid.");
        }
        this.begin = begin;
        this.end = end;
        this.vehicle = new SubscribedVehicle(plate, this);
    }

    // Methods
    public boolean isValid(){
        if ( end.isBeforeThan(Date.getToday()) ) return false;
        if ( begin.isAfterThan(Date.getToday()) ) return false;
        return true;
    }


    // Getter Setter
    public SubscribedVehicle getVehicle() {
        return vehicle;
    }
}
