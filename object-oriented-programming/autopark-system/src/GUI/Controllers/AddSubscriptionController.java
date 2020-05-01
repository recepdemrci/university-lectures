package GUI.Controllers;

import data.Date;
import data.Exceptions.InvalidDateException;
import data.Exceptions.SubscriptionTimeException;
import data.Subscription;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;


public class AddSubscriptionController {

    private Subscription aSubscription;

    @FXML
    private TextField plate;
    @FXML
    private TextField beginDay;
    @FXML
    private TextField beginMonth;
    @FXML
    private TextField beginYear;
    @FXML
    private TextField endDay;
    @FXML
    private TextField endMonth;
    @FXML
    private TextField endYear;

    @FXML
    private Label info;

    @FXML
    public void createSubscription() throws Exception {
        if(!checkSubscription()){
            info.setText("> Please fill in all fields");
            return;
        }

        try {
        Date beginDate = new Date(Integer.decode(beginDay.getText()), Integer.decode(beginMonth.getText()), Integer.decode(beginYear.getText()));
        Date endDate = new Date(Integer.decode(endDay.getText()), Integer.decode(endMonth.getText()), Integer.decode(endYear.getText()));
        aSubscription = new Subscription(beginDate, endDate, plate.getText());

        FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("../MainPage.fxml"));
        fxmlLoader.load();

        MainPageController controller = fxmlLoader.getController();
        controller.setSubscription(aSubscription);

        controller.closeStage();
        }
        catch (InvalidDateException e) {info.setText("> Please check the dates");}
        catch (SubscriptionTimeException s){info.setText("> Invalid subscription date");}
    }

    boolean checkSubscription(){
        if(!plate.getText().isEmpty() && !beginDay.getText().isEmpty() && !beginMonth.getText().isEmpty() && !beginYear.getText().isEmpty() &&
            !endDay.getText().isEmpty() && !endMonth.getText().isEmpty() && !endYear.getText().isEmpty()){
            return true;
        }
        return false;
    }
}
