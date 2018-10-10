
import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.HPos;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.geometry.VPos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Spinner;
import javafx.scene.control.SpinnerValueFactory;
import javafx.scene.control.TextField;
import javafx.scene.control.Tooltip;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.scene.text.Text;
import javafx.stage.Stage;

public class Kalah extends Application {
	
	final int houses = 6;
	final int seeds = 4;

	@Override
	public void start(Stage stage) throws Exception {
		stage.setTitle("Kalah");
		Scene scene = new Scene(menuGUI(stage), 500, 450);
		stage.setScene(scene);
		stage.show();
	}

	public void game(Stage stage, Board board) throws Exception {
		stage.setTitle("Kalah");
		Scene scene = new Scene(boardGUI(stage, board), 700, 450);
		stage.setScene(scene);
		stage.show();
	}
	
	public BorderPane menuGUI(Stage stage) {
		BorderPane border = new BorderPane();
		
		Label diffLabel = new Label("Select difficulty:");
		Tooltip tooltip = new Tooltip();
		tooltip.setText("Only for playing\n" + "against AI\n");
		ObservableList<String> difficulties = FXCollections.observableArrayList("Easy", "Medium", "Hard");
		final Spinner<String> diffSpinner = new Spinner<String>();
	    SpinnerValueFactory<String> diffFactory = new SpinnerValueFactory.ListSpinnerValueFactory<String>(difficulties);
	    diffFactory.setValue("Medium");
	    diffSpinner.setValueFactory(diffFactory);
	    diffSpinner.setTooltip(tooltip);
	    
	    Label houseLabel = new Label("Number of houses:");
	    final Spinner<Integer> houseSpinner = new Spinner<Integer>();
	    SpinnerValueFactory<Integer> houseFactory = new SpinnerValueFactory.IntegerSpinnerValueFactory(4, 9, 6);
	    houseSpinner.setValueFactory(houseFactory);
	    
	    Label seedLabel = new Label("Number of seeds:");
	    final Spinner<Integer> seedSpinner = new Spinner<Integer>();
	    SpinnerValueFactory<Integer> seedFactory = new SpinnerValueFactory.IntegerSpinnerValueFactory(1, 10, 4);
	    seedSpinner.setValueFactory(seedFactory);
	    
	    Button pvpButton = new Button();
		pvpButton.setText("Player vs Player");
		pvpButton.setOnAction(new EventHandler<ActionEvent>() {
			 
            @Override
            public void handle(ActionEvent event) {
                stage.hide();
                Board board = new Board();
                try {
					game(stage, board);
				} catch (Exception e) {
					e.printStackTrace();
				}
            }
        });
		
		Button pvaiButton = new Button();
		pvaiButton.setText("Player vs AI");
		//need to add handler for AI game
	    
		GridPane title = new GridPane();
		title.setAlignment(Pos.CENTER);
		title.setHgap(10);
		title.setVgap(10);
		title.setPadding(new Insets(10, 10, 10, 10));
		Text scenetitle = new Text("Kalah");
		scenetitle.setFont(Font.font("Arial", FontWeight.NORMAL, 20));
		Text description = new Text("Team Project 2 - CSCE 315");
		description.setFont(Font.font("Arial", FontWeight.NORMAL, 16));
		Text members = new Text("Matt Breitling, Alex Kaiser, Noe Balli");
		members.setFont(Font.font("Arial", FontWeight.NORMAL, 12));
		title.setHalignment(scenetitle, HPos.CENTER);
		title.setHalignment(description, HPos.CENTER);
		title.setHalignment(members, HPos.CENTER);
		title.add(scenetitle, 0, 0, 2, 1);
		title.add(description, 0, 1);
		title.add(members, 0, 2);
		
		VBox settings = new VBox();
		settings.setAlignment(Pos.BASELINE_CENTER);
		settings.setPadding(new Insets(15, 15, 15, 15));
		settings.setSpacing(10);
		settings.getChildren().addAll(diffLabel, diffSpinner, houseLabel, houseSpinner, seedLabel, seedSpinner);
		
		HBox buttons = new HBox();
		buttons.setAlignment(Pos.BASELINE_RIGHT);
		buttons.setPadding(new Insets(15, 12, 15, 12));
		buttons.setSpacing(10);
		buttons.getChildren().addAll(pvpButton, pvaiButton);
		
		border.setTop(title);
		border.setBottom(buttons);
		border.setCenter(settings);
		
		return border;
	}

	public GridPane boardGUI(Stage stage, Board board) {
		
		GridPane grid = new GridPane();
		grid.setHgap(10);
	    grid.setVgap(10);
	    grid.setPadding(new Insets(100, 10, 0, 10));

		TextField pit1 = new TextField();
		pit1.setDisable(true);
		pit1.setText(Integer.toBinaryString(board.getPits().get(houses)));
		Label player1 = new Label("Player 1");
		TextField pit2 = new TextField();
		pit2.setDisable(true);
		pit2.setText(Integer.toBinaryString(board.getPits().get(houses*2+1)));
		Label player2 = new Label("Player 2");
		grid.add(pit2, 0, 2);
		grid.add(player2, 0, 1);
		grid.add(pit1, houses+1, 2);
		grid.add(player1, houses+1, 3);
	    
		TextField[] houseArray = new TextField[houses*2+1];
		Button[] buttonArray = new Button[houses*2+1];
		for(int i = 0; i < houses; i++) {
			houseArray[i] = new TextField();
			houseArray[i].setDisable(true);
			houseArray[i].setText(Integer.toString(board.getPits().get(i)));
			houseArray[i].setAlignment(Pos.CENTER);
			houseArray[i+houses-1] = new TextField();
			houseArray[i+houses-1].setDisable(true);
			houseArray[i+houses-1].setText(Integer.toString(board.getPits().get(houses*2-i)));
			houseArray[i+houses-1].setAlignment(Pos.CENTER);
			grid.add(houseArray[i], i+1, 3);
			grid.add(houseArray[i+houses-1], i+1, 1);
			
			buttonArray[i] = new Button();
			int index = i;
			buttonArray[i].setOnAction(new EventHandler<ActionEvent>() {
				@Override
				public void handle(ActionEvent event) {
					try {
						board.pick(index);
						updateHouses(houseArray, pit1, pit2, board);
						
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			});
			grid.add(buttonArray[i], i+1, 4);
			buttonArray[i+houses-1] = new Button();
			buttonArray[i+houses-1].setOnAction(new EventHandler<ActionEvent>() {
				@Override
				public void handle(ActionEvent event) {
					try {
						board.pits.set(index+houses-1,99);
						updateHouses(houseArray, pit1, pit2, board);
						
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			});
			grid.add(buttonArray[i+houses-1], i+1, 0);
		}
		
		return grid;
	}
	
	public void updateHouses(TextField[] houseArray, TextField pit1, TextField pit2, Board board) {
		for(int i = 0; i < houses; i++) {
			houseArray[i].setText(Integer.toString(board.getPits().get(i)));
			houseArray[i+houses+1].setText(Integer.toString(board.getPits().get(houses*2-i)));
			pit1.setText(Integer.toBinaryString(board.getPits().get(houses)));
			pit2.setText(Integer.toBinaryString(board.getPits().get(houses*2+1)));
		}
	}
	
	public static void main(String[] args) {
		launch(args);
	}
	
}
