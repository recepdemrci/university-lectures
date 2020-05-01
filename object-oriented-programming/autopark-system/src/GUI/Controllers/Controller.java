package GUI.Controllers;

import data.AutoPark;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.AnchorPane;
;


public class Controller {

    @FXML
    private AnchorPane rootPane;

    @FXML
    private TextField autoparkName;
    @FXML
    private TextField hourlyFee;
    @FXML
    private TextField capacity;
    @FXML
    private Label info;

    @FXML
    public void login() throws Exception{
        if(!checkLogin()){
            info.setText("> Please fill in all fields");
            return;
        }

        FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("../MainPage.fxml"));
        fxmlLoader.load();

        MainPageController controller = fxmlLoader.getController();
        controller.init(new AutoPark(autoparkName.getText(),Double.parseDouble(hourlyFee.getText()),Integer.decode(capacity.getText())));

        loadPage("../MainPage.fxml");
    }

    boolean checkLogin(){
        if(!hourlyFee.getText().isEmpty() && !capacity.getText().isEmpty()){
            if(Double.parseDouble(hourlyFee.getText()) >= 0 && Integer.decode(capacity.getText()) > 0){
                return true;
            }
        }
        return false;
    }
    public void loadPage(String location) throws Exception {
        AnchorPane pane = FXMLLoader.load(getClass().getResource(location));
        rootPane.getChildren().setAll(pane);
    }
}
