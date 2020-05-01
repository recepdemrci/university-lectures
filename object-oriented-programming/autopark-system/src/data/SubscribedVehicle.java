package data;

public class SubscribedVehicle implements Vehicle {

    private String plate;
    private Subscription subscription;


    // Constructors
    public SubscribedVehicle(String plate, Subscription subscription){
        this.plate = plate;
        this.subscription = subscription;
    }

    // Methods
    public String getPlate(){
        return plate;
    }
    public Subscription getSubscription(){
        return subscription;
    }
    public boolean isSpecial(){
        return false;
    }

    @Override
    public String toString() {
        return "<"+plate+">";
    }
}
