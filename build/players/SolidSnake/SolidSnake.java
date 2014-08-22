import java.util.Random;

public class SolidSnake {

    public static void main(String[] args) {
        int target;
        char hp;
        Random rand = new Random();

        if(args.length == 0) {
            target = rand.nextInt(3) + 1;
            System.out.println("D"+target);
            return;
        }

        String[] list = args[1].split(" ");

        for(int i = 1; i <= list.length; ++i) {
            hp = list[i].charAt(0);
            if(hp > 0) {
                char c = list[i].charAt(list[i].length()-1);
                String lastMove = String.valueOf(c);

                //Check for P
                if(lastMove.equals('P')) {
                    target = i;
                    System.out.println("S"+target);
                    return;
                }

                //Check for S*
                if(Character.isDigit(lastMove.charAt(lastMove.length() - 1))) {
                    target = i;
                    System.out.println("S"+target);
                    return;
                }
            }
        }

        //If not any P or S*
        for(int i = 1; i <= list.length; ++i) {
            hp = list[i].charAt(0);
            if(hp > 0) {
                target=i;
                System.out.print("D"+target);
                return;
            }
        }

        //Catch-safe
        System.out.print("D1");
        return;
    }

}
