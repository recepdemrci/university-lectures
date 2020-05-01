package data;

public class OfficalVehicle implements Vehicle {

    private String plate;

    // Constructors
    public OfficalVehicle(String plate){
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
        return true;
    }

    @Override
    public String toString() {
        return "*"+plate+"*";
    }
}
