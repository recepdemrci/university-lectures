package data;

public class RegularVehicle implements Vehicle {

    private String plate;

    // Constructors
    public RegularVehicle(String plate){
        this.plate = plate;
    }

    // Methods
    public String getPlate(){
        return plate;
    }
    public Subscription getSubscription(){
        return null;
    }
    public boolean isSpecial(){
        return false;
    }

    @Override
    public String toString() {
        return plate;
    }
}
