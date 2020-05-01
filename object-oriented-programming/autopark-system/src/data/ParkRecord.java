package data;

import data.Exceptions.ExitTimeException;

public class ParkRecord {
    private Time enterTime, exitTime;
    private Vehicle vehicle;

    // Constructors
    public ParkRecord(Time enterTime, Vehicle vehicle) {
        this.enterTime = enterTime;
        this.vehicle = vehicle;
    }

    // Methods
    public int getParkDuration(){
        int duration = (enterTime.getDifference(exitTime) / 60);
        if(enterTime.getDifference(exitTime)%60!=0) {duration++;}
        return duration;
        //  0 DK - 60 DK = 1 saat
        // 61 DK - 120DK = 2 saat gibi gibi
    }

    @Override
    public String toString() {
        return "ParkRecord{" +
                "enterTime=" + enterTime +
                ", exitTime=" + exitTime +
                ", vehicle=" + vehicle +
                "}";
    }

    // Getter Setter
    public Vehicle getVehicle(){
        return vehicle;
    }

    public Time getEnterTime() {
        return enterTime;
    }

    public Time getExitTime() {
        return exitTime;
    }

    public void setExitTime(Time exitTime) throws ExitTimeException {
       if(enterTime.isAfterThan(exitTime)){
           throw new ExitTimeException("Exit time must be after the enter time.");
       }
           this.exitTime = exitTime;

    }

    // Getters for tableview
    public String getVehiclexd(){
        return vehicle.getPlate();
    }
    public String getExitTimexd() { return exitTime.toString(); }
    public String getEnterTimexd() { return enterTime.toString(); }

}
