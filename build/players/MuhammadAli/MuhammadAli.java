import java.util.ArrayList;
import java.util.Random;

public class MuhammadAli {

    public static void main(String[] args) {
        if(args.length < 2)
        {
            return; // nothing to do here
        }
        String[] players = args[1].split(" ");
        if(players.length < 4 || !isAlive(players[0]))
        {
            return; // nothing to do here
        }
        Random rand = new Random();

        int turn = players[0].split(",").length;

        int highestHealth = -1;
        int healthiestPlayer = -1; 
        int livingPlayers = 0;
        for(int i=1; i<4; i++) // who is doing too well for their own good?
        {
            int health = getHealth(players[i]);
            if(health>=0)
            {
                livingPlayers++;
            }
            if(health>highestHealth || (health==highestHealth && rand.nextDouble()>0.5))
            {
                highestHealth = health;
                healthiestPlayer = i;
            }
        }

        if(livingPlayers==1 && ((turn==49 && highestHealth<2) || grenadeExplodes(players[healthiestPlayer])>-1))
        {
            System.out.print("S" + Integer.toString(healthiestPlayer).charAt(0)); // you had it coming dude
        }

        if(turn!=49)
        {
            int mostShots = -1;
            int triggerHappyPlayer = -1;        
            for(int i=1; i<4; i++) // who's the most trigger happy player?
            {
                int shots = shotsTaken(players[i]);
                if(canMove(players[i]) && (shots>=mostShots))
                {
                    if(shots>mostShots || ((grenadeExplodes(players[i])==-1 || rand.nextDouble()>0.5) && grenadeExplodes(players[triggerHappyPlayer])==-1)) // if you're holding a grenade, you'll have other things on your mind than shooting.
                    mostShots = shots;
                    triggerHappyPlayer = i;
                }           
            }

            if(mostShots>0 || turn < 2)
            {
                System.out.print("D" + Integer.toString(triggerHappyPlayer)); // you didn't get your hopes up, did you?
                return;
            }
        }

        if(highestHealth>2 && turn==49)
        {
            System.out.print("S0"); // Goodbye, cruel world
        }
        else
        {
            System.out.print("S" + Integer.toString(healthiestPlayer).charAt(0)); // seems like I can take a free shot at someone
        }

    }

    public static boolean isAlive(String player) 
    {
        return player.charAt(0)!='-'; 
    }

    public static boolean canMove(String player)
    {
        return isAlive(player) && (player.length() == 1 || player.charAt(player.length()-2)!='S');
    }

    public static int grenadeExplodes(String player)
    {
        String[] actions = player.split(",");

        if(actions.length>3 && actions[actions.length - 3].charAt(0)=='P' 
            && actions[actions.length - 2].charAt(0)=='T' 
            && actions[actions.length - 1].charAt(0)=='P')
        {
            return 0;
        } 
        else if(actions.length>2 && actions[actions.length - 2].charAt(0)=='P' 
            && actions[actions.length - 1].charAt(0)=='T')
        {
            return 1;
        } 
        else if(actions.length>1 && actions[actions.length - 1].charAt(0)=='P')
        {
            return 2;
        }
        else
        {
            return -1;
        }
    }

    public static int shotsTaken(String player)
    {
        String[] actions = player.split(",");
        int shots = 0;
        for(int i=0; i<actions.length; i++)
        {
            if(actions[i].charAt(0)=='S')
            {
                shots ++;
            }
        }

        return shots;
    }

    public static int getHealth(String player)
    {
        return Integer.parseInt(player.split(",")[0]);
    }
}
