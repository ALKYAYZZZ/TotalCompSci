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
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundImage;
import javafx.scene.layout.BackgroundPosition;
import javafx.scene.layout.BackgroundRepeat;
import javafx.scene.layout.BackgroundSize;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.scene.text.Text;
import javafx.stage.Stage;

enum gametype{
	ONEPLAYER, TWOPLAYER, CLIENT, SERVER;
}

public class Kalah extends Application {
	//Title menu
	public void start(Stage stage) throws Exception {
		stage.setTitle("Kalah");
		Scene scene = new Scene(titleGUI(stage), 500, 450);
		stage.setScene(scene);
		stage.show();
	}
	
	//Settings menu
	public void menuStart(Stage stage, gametype game) throws Exception {
		stage.setTitle("Kalah");
		Scene scene = new Scene(menuGUI(stage, game), 500, 600);
		stage.setScene(scene);
		stage.show();
	}
	
	//Game interface
	public void gameStart(Stage stage, Game game) throws Exception {
		stage.setTitle("Kalah");
		Scene scene = new Scene(boardGUI(stage, game), game.getHouses()*95+50, 400);
		stage.setScene(scene);
		stage.show();
	}
	
	
	//Layout of title menu
	@SuppressWarnings("static-access")
	public BorderPane titleGUI(Stage stage) {
		BorderPane border = new BorderPane();
		
		Button pvpButton = new Button();
		pvpButton.setText("Two Player");
		pvpButton.setOnAction(new EventHandler<ActionEvent>() {	
			
            @Override
            public void handle(ActionEvent event) {            	
                stage.hide();
                gametype game = gametype.TWOPLAYER;
				try {
					menuStart(stage, game);
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
            }
        });
		
		Button pvaiButton = new Button();
		pvaiButton.setText("One Player");
		pvaiButton.setOnAction(new EventHandler<ActionEvent>() {	
			
            @Override
            public void handle(ActionEvent event) {            	
                stage.hide();
                gametype game = gametype.ONEPLAYER;
                try {
					menuStart(stage, game);
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
            }
        });
		
		Button clientButton = new Button();
		clientButton.setText("Start Client");
		clientButton.setOnAction(new EventHandler<ActionEvent>() {	
			
            @Override
            public void handle(ActionEvent event) {            	
                stage.hide();
                gametype game = gametype.CLIENT;
                try {
					menuStart(stage, game);
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
            }
        });
		
		Button serverButton = new Button();
		serverButton.setText("Start Server");
		serverButton.setOnAction(new EventHandler<ActionEvent>() {	
			
            @Override
            public void handle(ActionEvent event) {            	
                stage.hide();
                gametype game = gametype.SERVER;
                try {
					menuStart(stage, game);
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
		
		VBox buttons = new VBox();
		buttons.setAlignment(Pos.BASELINE_CENTER);
		buttons.setPadding(new Insets(15, 12, 15, 12));
		buttons.setSpacing(10);
		buttons.getChildren().addAll(pvaiButton, pvpButton, clientButton, serverButton);
		
		border.setTop(title);
		border.setCenter(buttons);
		
		return border;
	}
	
	//Layout of settings menu
	@SuppressWarnings("static-access")
	public BorderPane menuGUI(Stage stage, gametype _gametype) {
		BorderPane border = new BorderPane();
	    
	    Label houseLabel = new Label("Number of houses:");
	    final Spinner<Integer> houseSpinner = new Spinner<Integer>();
	    SpinnerValueFactory<Integer> houseFactory = new SpinnerValueFactory.IntegerSpinnerValueFactory(4, 9, 6);
	    houseSpinner.setValueFactory(houseFactory);
	    
	    Label seedLabel = new Label("Number of seeds:");
	    final Spinner<Integer> seedSpinner = new Spinner<Integer>();
	    SpinnerValueFactory<Integer> seedFactory = new SpinnerValueFactory.IntegerSpinnerValueFactory(1, 10, 4);
	    seedSpinner.setValueFactory(seedFactory);
	    
	    CheckBox randomSeed = new CheckBox("Random seed");
	    CheckBox homeTeam = new CheckBox("Home team first");
	    homeTeam.setSelected(true);	    

		Label diffLabel = new Label("Select difficulty:");
		Tooltip tooltip = new Tooltip();
		tooltip.setText("Only for playing\n" + "against AI\n");
		ObservableList<String> difficulties = FXCollections.observableArrayList("Easy", "Medium", "Hard");
		final Spinner<String> diffSpinner = new Spinner<String>();
	    SpinnerValueFactory<String> diffFactory = new SpinnerValueFactory.ListSpinnerValueFactory<String>(difficulties);
	    diffFactory.setValue("Medium");
	    diffSpinner.setValueFactory(diffFactory);
	    diffSpinner.setTooltip(tooltip);
	    
	    Label clientLabel = new Label("Client type:");
		Tooltip clientTooltip = new Tooltip();
		clientTooltip.setText("Connect to server as AI or as player");
		ObservableList<String> clients = FXCollections.observableArrayList("Player", "AI");
		final Spinner<String> clientSpinner = new Spinner<String>();
	    SpinnerValueFactory<String> clientFactory = new SpinnerValueFactory.ListSpinnerValueFactory<String>(clients);
	    clientFactory.setValue("Player");
	    clientSpinner.setValueFactory(clientFactory);
	    clientSpinner.setTooltip(clientTooltip);
		Label hostLabel = new Label("Host name:");
	    TextField host = new TextField();
	    Label portLabel = new Label("Port:");
	    TextField port = new TextField();
	    
	    Label serverLabel = new Label("Server type:");
		Tooltip serverTooltip = new Tooltip();
		serverTooltip.setText("Start server as AI, player, or host");
		ObservableList<String> servers = FXCollections.observableArrayList("Player", "AI", "Host");
		final Spinner<String> serverSpinner = new Spinner<String>();
	    SpinnerValueFactory<String> serverFactory = new SpinnerValueFactory.ListSpinnerValueFactory<String>(servers);
	    serverFactory.setValue("Player");
	    serverSpinner.setValueFactory(serverFactory);
	    serverSpinner.setTooltip(serverTooltip);
		
		Button startButton = new Button();
		startButton.setText("Start");
		startButton.setOnAction(new EventHandler<ActionEvent>() {	
			
            @Override
            public void handle(ActionEvent event) {            	
                stage.hide();
                
                try {
                	int numericDifficulty;
    				if(diffSpinner.getValue() == "Easy"){
    					numericDifficulty = 1;
    				}
    				if(diffSpinner.getValue() == "Medium"){
    					numericDifficulty = 2;
    				}
    				else{
    					numericDifficulty = 3;
    				}
                    Board board = new Board(seedSpinner.getValue(), houseSpinner.getValue(), randomSeed.isSelected());
                    Game game = null;
                    if(_gametype == gametype.TWOPLAYER) {
                    	System.out.print("made it here");
                    	game = new Game(board, homeTeam.isSelected());
                    	System.out.print("made it here");
                    }
                    if(_gametype == gametype.ONEPLAYER) {
                    	System.out.println("Made it here.");
            			game = new Game(board, homeTeam.isSelected(), true, numericDifficulty);
                    	System.out.println("Made it here.");
            		}
            		if(_gametype == gametype.CLIENT) {
            			if(clientSpinner.getValue() == "AI") {
            				game = new Game(board, true, numericDifficulty, host.getText(), Integer.parseInt(port.getText()));
            			}
            			else {
            				game = new Game(board, true, numericDifficulty, host.getText(), Integer.parseInt(port.getText()));
            			}
            		}
            		if(_gametype == gametype.SERVER) {
            			if(serverSpinner.getValue() == "Host") {
            				game = new Game(houseSpinner.getValue(), seedSpinner.getValue(), randomSeed.isSelected(), (long) 15, numericDifficulty, true, Integer.parseInt(port.getText()));
            			}
            			else if(serverSpinner.getValue() == "AI") {
            				game = new Game(houseSpinner.getValue(), seedSpinner.getValue(), randomSeed.isSelected(), (long) 15, 0, true, numericDifficulty, false, Integer.parseInt(port.getText()));
            			}
            			else {
            				game = new Game(houseSpinner.getValue(), seedSpinner.getValue(), randomSeed.isSelected(), (long) 15, 0, false, numericDifficulty, false, Integer.parseInt(port.getText()));
            			}
            		}
					gameStart(stage, game);
				}
                catch (Exception e) {
					
				}
            }
        });
		
		Button returnButton = new Button();
		returnButton.setText("Back");
		returnButton.setOnAction(new EventHandler<ActionEvent>() {	
			
            @Override
            public void handle(ActionEvent event) {            	
                stage.hide();
                try {
					start(stage);
				} catch (Exception e) {
					
				}
            }
        });
		
		VBox settings = new VBox();
		settings.setAlignment(Pos.BASELINE_CENTER);
		settings.setPadding(new Insets(15, 15, 15, 15));
		settings.setSpacing(10);
		settings.getChildren().addAll(houseLabel, houseSpinner, seedLabel, seedSpinner, randomSeed);
		if(_gametype == gametype.TWOPLAYER) {
			settings.getChildren().addAll(homeTeam);
		}
		if(_gametype == gametype.ONEPLAYER) {
			settings.getChildren().addAll(homeTeam, diffLabel, diffSpinner);
		}
		if(_gametype == gametype.CLIENT) {
			settings.getChildren().addAll(clientLabel, clientSpinner, hostLabel, host, portLabel, port);
		}
		if(_gametype == gametype.SERVER) {
			settings.getChildren().addAll(serverLabel, serverSpinner, portLabel, port);
		}
		
		
		HBox buttons = new HBox();
		buttons.setAlignment(Pos.BASELINE_CENTER);
		buttons.setPadding(new Insets(15, 12, 15, 12));
		buttons.setSpacing(10);
		buttons.getChildren().addAll(startButton, returnButton);
		
		border.setBottom(buttons);
		border.setCenter(settings);
		
		return border;
	}
	
	//Layout of game interface
	@SuppressWarnings("static-access")
	public GridPane boardGUI(Stage stage, Game game) {
		
		int houses = game.getHouses();
		
		GridPane grid = new GridPane();
		grid.setHgap(10);
	    grid.setVgap(10);
	    grid.setPadding(new Insets(25, 10, 0, 10));
	    
	    Image pit = new Image("pit.png", 64, 64, true, true);
	    ImageView pit1Icon = new ImageView(pit);
		Text pit1 = new Text();
		pit1.setDisable(true);
		pit1.setText(Integer.toString(game.getPit(houses)));
		StackPane pit1Stack = new StackPane();
		pit1Stack.getChildren().addAll(pit1Icon, pit1);
		Label player1 = new Label("Player 1");
		ImageView pit2Icon = new ImageView(pit);
		Text pit2 = new Text();
		pit2.setDisable(true);
		pit2.setText(Integer.toString(game.getPit(houses*2+1)));
		StackPane pit2Stack = new StackPane();
		pit2Stack.getChildren().addAll(pit2Icon, pit2);
		Label player2 = new Label("Player 2");
		grid.add(pit2Stack, 0, 2);
		grid.add(player2, 0, 1);
		grid.add(pit1Stack, houses+1, 2);
		grid.add(player1, houses+1, 3);
	    
		Text prompt = new Text("Player 1's turn!");
		prompt.setFont(Font.font("Arial", FontWeight.NORMAL, 20));
		grid.add(prompt, houses/2, 6, 2, 1);
		
		ImageView[] pitIcon = new ImageView[houses*2+1]; 
		Text[] houseArray = new Text[houses*2+1];
		StackPane[] houseStack = new StackPane[houses*2+1];
		Button[] buttonArray = new Button[houses*2+1];
		for(int i = 0; i < houses; i++) {
			
			houseArray[i] = new Text();
			houseArray[i].setDisable(true);
			houseArray[i].setText(Integer.toString(game.getPit(i)));
			pitIcon[i] = new ImageView(pit);
			houseStack[i] = new StackPane();
			houseStack[i].getChildren().addAll(pitIcon[i], houseArray[i]);
			
			houseArray[i+houses] = new Text();
			houseArray[i+houses].setDisable(true);
			houseArray[i+houses].setText(Integer.toString(game.getPit(houses*2-i)));
			pitIcon[i+houses] = new ImageView(pit);
			houseStack[i+houses] = new StackPane();
			houseStack[i+houses].getChildren().addAll(pitIcon[i+houses], houseArray[i+houses]);
			
			grid.add(houseStack[i], i+1, 3);
			grid.add(houseStack[i+houses], i+1, 1);
			
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
		
		Button swap = new Button();
		swap.setText("Swap sides");
		swap.setMaxSize(Double.MAX_VALUE, Double.MAX_VALUE);
		swap.setDisable(true);
		swap.setOnAction(new EventHandler<ActionEvent>() {
			@Override
			public void handle(ActionEvent event) {
				try {
					game.swap();
					game.swapped = true;
					swap.setDisable(true);
				} catch (Exception e) {
					
				}
			}
		});
		grid.add(swap, 0, 6, 2, 1);
		
		//Task to handle updates to board interface
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
		    				if(game.swapped == true) {
		    					swap.setDisable(true);
		    				}
		    				if(game.turnNumber == 1) {
		    					swap.setDisable(false);
		    				}
		    				if(game.turnNumber > 1) {
		    					swap.setDisable(true);
		    				}
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