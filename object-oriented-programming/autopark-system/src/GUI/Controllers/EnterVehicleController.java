package GUI.Controllers;


import data.Time;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.control.CheckBox;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;


public class EnterVehicleController {

    @FXML
    private TextField plate;
    @FXML
    private CheckBox isOfficial;
    @FXML
    private Label info;

    @FXML
    public void entersVehicle() throws Exception {
        if(!checkEnterVehicle()){
            info.setText("> Please fill in the field");
            return;
        }

        FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("../MainPage.fxml"));
        fxmlLoader.load();

        MainPageController controller = fxmlLoader.getController();
        controller.getaAutoPark().vehicleEnters(plate.getText(), Time.getTime(), isOfficial.isSelected());

        controller.closeStage();
    }

    boolean checkEnterVehicle(){
        if(!plate.getText().isEmpty()){
            return true;
        }
        return false;
    }

}
