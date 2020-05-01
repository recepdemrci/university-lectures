package data.TestClasses;

import data.Date;
import data.Subscription;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

class SubscriptionTest {

    @Test
    void isValid_True() throws Exception {
        Date begin = new Date(2, 4, 2019);
        Date end = new Date(2, 5, 2020);
        Subscription subscription = new Subscription(begin, end, "123");

        boolean result = subscription.isValid();
        assertTrue(result);
    }

    @Test
    void isValid_False() throws Exception {
        Date begin = new Date(2, 4, 2019);
        Date end = new Date(2, 5, 2019);
        Subscription subscription = new Subscription(begin, end, "123");

        boolean result = subscription.isValid();
        assertFalse(result);
    }

}