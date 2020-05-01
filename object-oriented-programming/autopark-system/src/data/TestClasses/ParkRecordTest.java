package data.TestClasses;

import data.ParkRecord;
import data.RegularVehicle;
import data.Time;
import data.Vehicle;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class ParkRecordTest {

    @Test
    void getParkDuration() throws Exception {
        Time enterTime = new Time(12,57);
        Time exitTime = new Time(23,56);

        Vehicle aVehicle = new RegularVehicle("Vehicle#123");

        ParkRecord aParkRecord = new ParkRecord(enterTime, aVehicle);
        aParkRecord.setExitTime(exitTime);

        int result = aParkRecord.getParkDuration();
        assertEquals(11,result);
    }
}