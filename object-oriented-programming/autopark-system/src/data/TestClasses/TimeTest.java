package data.TestClasses;

import data.Time;
import org.junit.jupiter.api.Test;


import static org.junit.jupiter.api.Assertions.*;

class TimeTest {

    @Test
    void getTime() {
        String result = Time.getTime().toString();
        String expected = "(17:28)";                    // O anki zamana ayarlanmalı

        assertEquals(expected, result);
    }

    @Test
    void getDifference() throws Exception{
        Time enterTime = new Time(12,25);
        Time exitTime = new Time(13,30);

        int result = enterTime.getDifference(exitTime);
        int expected = 65;

        assertEquals(expected, result);
    }

    @Test
    void isAfterThan() throws Exception {
        Time now = new Time(12,57);
        Time before = new Time(11,45);
        Time after = new Time(15,23);

        assertTrue(after.isAfterThan(now));
        assertFalse(before.isAfterThan(now));
    }
}