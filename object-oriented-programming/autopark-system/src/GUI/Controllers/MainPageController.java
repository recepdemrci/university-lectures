package GUI.Controllers;

import data.AutoPark;
import data.Subscription;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;



public class MainPageController {

    private static AutoPark aAutoPark;
    private static Stage stage;

    public void init(AutoPark aAutoPark){
        this.aAutoPark = aAutoPark;

        System.out.println(aAutoPark.toString());
    }

    @FXML
    public void clickedAddVehicle() throws Exception {
        FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("../AddSubscription.fxml"));
        Parent root = fxmlLoader.load();
        stage = new Stage();
        stage.setTitle("Subscription");
        stage.setScene(new Scene(root));
        stage.show();
        System.out.println(aAutoPark.toString());
    }

    @FXML
    public void clickedEnterVehicle() throws Exception {
        FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("../EnterVehicle.fxml"));
        Parent root = fxmlLoader.load();
        stage = new Stage();
        stage.setTitle("Enter Vehicle");
        stage.setScene(new Scene(root));
        stage.show();
        System.out.println(aAutoPark.toString());
    }

    public void closeStage(){
        stage.close();
    }

    public AutoPark getaAutoPark() {
        return aAutoPark;
    }

    public void setSubscription(Subscription subscription) {
        aAutoPark.addVehicle(subscription.getVehicle());
    }

}
