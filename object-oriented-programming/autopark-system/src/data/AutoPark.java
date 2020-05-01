package data;

import data.Exceptions.EnterTimeException;
import data.Exceptions.ExitTimeException;

import java.util.Arrays;

public class AutoPark {
    private String autoparkName;
    private SubscribedVehicle[] subscribedVehicles;
    private ParkRecord[] parkRecords;
    private double hourlyFee, incomeDaily;
    private int capacity, indexVehicle, indexRecord;

    // Constructors
    public AutoPark(String autoparkName, double hourlyFee, int capacity) {
        this.autoparkName = autoparkName;
        this.hourlyFee = hourlyFee;
        this.capacity = capacity;
        this.subscribedVehicles = new SubscribedVehicle[capacity];
        this.parkRecords = new ParkRecord[99];
        this.indexVehicle = 0;
        this.indexRecord = 0;
    }

    // Methods
    public SubscribedVehicle searchVehicle(String plate){
        for ( int i = 0; i < indexVehicle; i++ ) {
            if(subscribedVehicles[i].getPlate().equals(plate) ){
                return subscribedVehicles[i];
            }
        }
        return null;
    }

    public boolean isParked(String plate){
        for(int i = 0; i < indexRecord; i++){
            if(parkRecords[i].getVehicle().getPlate().equals(plate)){
                if ( parkRecords[i].getExitTime() == null ) { return true; }
            }
        }
        return false;
    }

    public ParkRecord isParkedButReturnsParkRecord(String plate){
        for(int i = 0; i < indexRecord; i++){
            if(parkRecords[i].getVehicle().getPlate().equals(plate)){
                if ( parkRecords[i].getExitTime() == null ) { return parkRecords[i]; }
            }
        }
        return null;
    }

    public void enlargeVehicleArray() {
        SubscribedVehicle[] newSubscribedVehicles = new SubscribedVehicle[capacity+10];
        System.arraycopy(subscribedVehicles,0,newSubscribedVehicles,0,capacity);
        subscribedVehicles = newSubscribedVehicles;
        capacity += 10;
    }

    public boolean addVehicle( SubscribedVehicle vehicle ) {
        if ( searchVehicle( vehicle.getPlate() ) == null ) {
            if ( indexVehicle >= capacity ) {
                enlargeVehicleArray();
            }
            subscribedVehicles[indexVehicle] = vehicle;
            indexVehicle++;
            return true;
        }
        return false;
    }

    public boolean vehicleEnters( String plate, Time enterTime, boolean isOfficial) throws EnterTimeException {
        if(enterTime.isAfterThan(Time.getTime())){
            throw new EnterTimeException("Enter time can NOT be after than now");
        }

        if ( isParked(plate) ) {return false;}


        Vehicle vehicle = searchVehicle(plate);
        if ( vehicle == null ) {
            if ( isOfficial ) {
                vehicle = new OfficalVehicle( plate );
            } else {
                vehicle = new RegularVehicle( plate );
            }
        }
        else if(!vehicle.getSubscription().isValid()){
            vehicle = new RegularVehicle(plate);
        }

        parkRecords[indexRecord] = new ParkRecord(enterTime, vehicle);
        indexRecord++;
        return true;
    }

    public boolean vehicleExits( String plate, Time exitTime ) {
        if ( !isParked(plate) ) { return false; }

        ParkRecord theParkRecord = isParkedButReturnsParkRecord(plate);

        try{
            theParkRecord.setExitTime(exitTime);
        }catch (ExitTimeException t){t.printStackTrace();}

        if ( theParkRecord.getVehicle() instanceof RegularVehicle ) {
            incomeDaily += ( hourlyFee * theParkRecord.getParkDuration() );
        }
        return true;
    }

    public ParkRecord[] getParkRecords() {
        return parkRecords;
    }

    @Override
    public String toString() {
        return autoparkName +
                "\n\t subscribedVehicles=" + Arrays.toString(subscribedVehicles) +
                "\n\t parkRecords=" + Arrays.toString(parkRecords) +
                "\n\n\t hourlyFee=   " + hourlyFee +
                "\n\t incomeDaily= " + incomeDaily +
                "\n\t capacity=    " + capacity +
                "\n\t indexVehicle=" + indexVehicle +
                "\n\t indexRecord= " + indexRecord +
                "\n]";
    }
}
