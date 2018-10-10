import java.util.Scanner;

public abstract class Main extends Animal{

public String cry(){
    return "";
}

public static void main(String[] args){
    AnimalShelter as = new AnimalShelter();

    for(String arguments: args){
        as.addAnimal(arguments);
    }
    boolean exit = true;
    //Use switch for user decisions
    while(exit){
        Scanner scan = new Scanner(System.in);

        System.out.println("1: Add new animal");
        System.out.println("2: Adopt an animal");
        System.out.println("3: Adopt a cat");
        System.out.println("4: Adopt a dog ");
        System.out.println("5: Show animals in the shelter ");
        System.out.println("6: Show cats in the shelter ");
        System.out.println("7: Show dogs in the shelter ");
        System.out.println("0: Exit");
        System.out.print("Enter a number: ");

        int choice = scan.nextInt();

        switch(choice){
            case 0:
                exit = false;
                break;
            case 1:
                System.out.println("Enter first Animal Character Followed By Name.");
                String help = scan.next();
                help += scan.nextLine();
                as.addAnimal(help);
                break;
            case 2:
                System.out.println("Animal Adopted.");
                as.adopt();
                break;
            case 3:
                System.out.println("Cat Adopted.");
                as.adoptCat();
                break;
            case 4:
                System.out.println("Dog Adopted.");
                as.adoptDog();
                break;
            case 5:
                as.remainingAnimals();
                break;
            case 6:
                as.remainingCats();
                break;
            case 7:
                as.remainingDogs();
                break;
            default:
                break;
        }
        System.out.println(" ");

        }
    }
}