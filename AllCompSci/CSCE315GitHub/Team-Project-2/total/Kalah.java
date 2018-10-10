import boardPackage.Board;
import boardPackage.EMPTY_BOARD_EXCEPTION;
import boardPackage.INVALID_NUMBER_OF_PITS;
import boardPackage.INVALID_PIT_NUMBER;
import boardPackage.INVALID_STARTING_SEEDS;
import boardPackage.PICK_FROM_EMPTY_PIT;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.concurrent.Task;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.HPos;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.Label;
import javafx.scene.control.Spinner;
import javafx.scene.control.SpinnerValueFactory;
import javafx.scene.control.TextField;
import javafx.scene.control.Tooltip;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.scene.text.Text;
import javafx.stage.Stage;

public class Kalah extends Application {
	public void start(Stage stage) throws Exception {
		stage.setTitle("Kalah");
		Scene scene = new Scene(menuGUI(stage), 500, 450);
		stage.setScene(scene);
		stage.show();
	}

	public void gameStart(Stage stage, Game game) throws Exception {
		stage.setTitle("Kalah");
		Scene scene = new Scene(boardGUI(stage, game), game.getHouses()*120, 300);
		stage.setScene(scene);
		stage.show();
	}
	
	@SuppressWarnings("static-access")
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
	    
	    CheckBox randomSeed = new CheckBox("Random seed");
	    
	    Button pvpButton = new Button();
		pvpButton.setText("Player vs Player");
		pvpButton.setOnAction(new EventHandler<ActionEvent>() {	
			
            @Override
            public void handle(ActionEvent event) {            	
                stage.hide();
				try {
					Board board = new Board(seedSpinner.getValue(), houseSpinner.getValue(), randomSeed.isSelected());
					Game game = new Game(board, true, false);
					gameStart(stage, game);
				} catch (INVALID_STARTING_SEEDS e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				} catch (INVALID_NUMBER_OF_PITS e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
            }
        });
		
		Button pvaiButton = new Button();
		pvaiButton.setText("Player vs AI");
		pvaiButton.setOnAction(new EventHandler<ActionEvent>() {	
			
            @Override
            public void handle(ActionEvent event) {            	
                stage.hide();
				try {
					Board board = new Board(seedSpinner.getValue(), houseSpinner.getValue(), randomSeed.isSelected());
					Game game = new Game(board, true, true);
					gameStart(stage, game);
				} catch (INVALID_STARTING_SEEDS e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				} catch (INVALID_NUMBER_OF_PITS e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
            }
        });
	    
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
		settings.getChildren().addAll(diffLabel, diffSpinner, houseLabel, houseSpinner, seedLabel, seedSpinner, randomSeed);
		
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

	@SuppressWarnings("static-access")
	public GridPane boardGUI(Stage stage, Game game) {
		
		int houses = game.getHouses();
		
		GridPane grid = new GridPane();
		grid.setHgap(10);
	    grid.setVgap(10);
	    grid.setPadding(new Insets(25, 10, 0, 10));

		TextField pit1 = new TextField();
		pit1.setDisable(true);
		pit1.setText(Integer.toString(game.getPit(houses)));
		Label player1 = new Label("Player 1");
		TextField pit2 = new TextField();
		pit2.setDisable(true);
		pit2.setText(Integer.toString(game.getPit(houses*2+1)));
		Label player2 = new Label("Player 2");
		grid.add(pit2, 0, 2);
		grid.add(player2, 0, 1);
		grid.add(pit1, houses+1, 2);
		grid.add(player1, houses+1, 3);
	    
		Text prompt = new Text("Player 1's turn!");
		prompt.setFont(Font.font("Arial", FontWeight.NORMAL, 24));
		grid.add(prompt, houses/2, 6, 2, 1);
		
		TextField[] houseArray = new TextField[houses*2+1];
		Button[] buttonArray = new Button[houses*2+1];
		for(int i = 0; i < houses; i++) {
			houseArray[i] = new TextField();
			houseArray[i].setDisable(true);
			houseArray[i].setText(Integer.toString(game.getPit(i)));
			houseArray[i].setAlignment(Pos.CENTER);
			houseArray[i+houses] = new TextField();
			houseArray[i+houses].setDisable(true);
			houseArray[i+houses].setText(Integer.toString(game.getPit(houses*2-i)));
			houseArray[i+houses].setAlignment(Pos.CENTER);
			grid.add(houseArray[i], i+1, 3);
			grid.add(houseArray[i+houses], i+1, 1);
			
			int index = i;
			buttonArray[i] = new Button();
			buttonArray[i].setOnAction(new EventHandler<ActionEvent>() {
				@Override
				public void handle(ActionEvent event) {
					try {
						game.pick(index);
						
					} catch (Exception e) {
						
					}
				}
			});
			grid.setHalignment(buttonArray[i], HPos.CENTER);
			grid.add(buttonArray[i], i+1, 4);
			
			buttonArray[i+houses] = new Button();
			buttonArray[i+houses].setOnAction(new EventHandler<ActionEvent>() {
				@Override
				public void handle(ActionEvent event) {
					try {
						game.pick(houses*2-index);
						
					} catch (Exception e) {
						
					}
				}
			});
			GridPane.setHalignment(buttonArray[i+houses], HPos.CENTER);
			if(game.AIflag==false) {
				grid.add(buttonArray[i+houses], i+1, 0);
			}
		}
		
		Button menu = new Button();
		menu.setText("Return to menu");
		menu.setMaxSize(Double.MAX_VALUE, Double.MAX_VALUE);
		menu.setOnAction(new EventHandler<ActionEvent>() {
			@Override
			public void handle(ActionEvent event) {
				stage.hide();
				try {
					start(stage);
				} catch (Exception e) {
					
				}
			}
		});
		grid.add(menu, houses, 6, 2, 1);
		
		Task<Void> task = new Task<Void>() {
			@Override
			public Void call() throws Exception {
			    while (true) {
			    	Platform.runLater(new Runnable() {
			    		@Override
			    		public void run() {
			    			for(int i = 0; i < houses; i++) {
			    				houseArray[i].setText(Integer.toString(game.getPit(i)));
			    				houseArray[i+houses].setText(Integer.toString(game.getPit(houses*2-i)));
			    				buttonArray[i].setDisable(!game.turn);
			    				buttonArray[i+houses].setDisable(game.turn);
			    			}
		    				pit1.setText(Integer.toString(game.getPit(houses)));
		    				pit2.setText(Integer.toString(game.getPit(houses*2+1)));
		    				if(game.turn && !game.ended) {
		    					prompt.setText("Player 1's turn!");
		    				} else if (!game.turn && !game.AIflag && !game.ended) {
		    					prompt.setText("Player 2's turn!");
		    				} else if (!game.turn && game.AIflag && !game.ended){
		    					prompt.setText("AI's turn!");
		    					game.guiupdated = true;
		    				}
		    				try {
								game.ended();
							} catch (EMPTY_BOARD_EXCEPTION e) {
								game.ended=true;
							}
		    				if(game.ended) {
					    		if(game.getPit(houses) > game.getPit(houses*2+1)) {
					    			prompt.setText("Player 1 wins!");
					    		} else if (game.getPit(houses) < game.getPit(houses*2+1) && !game.AIflag) {
					    			prompt.setText("Player 2 wins!");
					    		} else if (game.getPit(houses) < game.getPit(houses*2+1) && game.AIflag) {
					    			prompt.setText("AI wins!");
					    		} else if (game.getPit(houses) == game.getPit(houses*2+1)){
					    			prompt.setText("It's a draw!");
					    		}
					    		for(int i = 0; i < houses; i++) {
				    				buttonArray[i].setDisable(true);
				    				buttonArray[i+houses].setDisable(true);
				    			}					    		
					    	}
			    		}
			    	});
			    	Thread.sleep(100);
			    	if(!game.turn && game.AIflag && game.guiupdated) {
			    		game.AImove();
			    		game.guiupdated = false;
			    	}
			    	
			    }
			}
		};
		Thread th = new Thread(task);
		th.setDaemon(true);
		th.start();
		
		return grid;
	}
	
	public static void main(String[] args) {
		launch(args);
	}
	
}
