package data.TestClasses;

import data.AutoPark;
import data.Date;
import data.Subscription;
import data.Time;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class AutoParkTest {

    private AutoPark yildiz;
    private Subscription[] subscriptions;
    private Time enterTime;
    private Time exitTime;

    @BeforeEach
    public void setUp() throws Exception {

        yildiz = new AutoPark("yildiz",100, 10);
        subscriptions = new Subscription[4];

        enterTime = new Time(9,26);
        exitTime = new Time(19,29);

        Date begin = new Date(2, 5, 2019);
        Date end = new Date(3, 5, 2019);

        subscriptions[0] = new Subscription(begin, end, "Vehicle#1");
        subscriptions[1] = new Subscription(begin, end, "Vehicle#2");
        subscriptions[2] = new Subscription(begin, end, "Vehicle#3");
        subscriptions[3] = new Subscription(begin, end, "Vehicle#4");

    }

    @Test
    void addVehicle_True() {
        assertTrue(yildiz.addVehicle(subscriptions[0].getVehicle()));
        assertFalse(yildiz.addVehicle(subscriptions[0].getVehicle()));
    }

    @Test
    void searchVehicle() {
        yildiz.addVehicle(subscriptions[0].getVehicle());
        yildiz.addVehicle(subscriptions[1].getVehicle());
        yildiz.addVehicle(subscriptions[2].getVehicle());

        assertSame(subscriptions[0].getVehicle(), yildiz.searchVehicle("Vehicle#1"));
        assertNull(yildiz.searchVehicle("Vehicle#5"));
    }

    @Test
    void isParked() throws Exception{
        yildiz.vehicleEnters("Vehicle#10",enterTime,false);

        assertTrue(yildiz.isParked("Vehicle#10"));
        assertFalse(yildiz.isParked("Vehicle#7"));
    }

    @Test
    void vehicleEnters() throws Exception {
        assertTrue(yildiz.vehicleEnters("Vehicle#1",enterTime,false));
        assertTrue(yildiz.vehicleEnters("Vehicle#5",enterTime, false));
        assertTrue(yildiz.vehicleEnters("Vehicle#6",enterTime, true));
        assertFalse(yildiz.vehicleEnters("Vehicle#1",enterTime,false));
    }

    @Test
    void vehicleExits() throws Exception {
        yildiz.vehicleEnters("Vehicle#2",enterTime,false);

        assertTrue(yildiz.vehicleExits("Vehicle#2",exitTime));
        assertFalse(yildiz.vehicleExits("Vehicle#2",exitTime));
    }
}