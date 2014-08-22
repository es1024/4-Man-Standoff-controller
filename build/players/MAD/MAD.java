    import java.util.ArrayList;
import java.util.Random;

public class MAD 
{
    public static void main(String[] args) 
    {
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

        int grenadeExplodes = grenadeExplodes(players[0]);        
        if(grenadeExplodes==-1)
        {
            System.out.print("P"); // I don't feel safe without a prepared grenade in my hand
            return;
        }

        int highestDamage = -1;
        int playerToShoot = -1;        
        for(int i=1; i<4; i++) // did anyone try to hit me?
        {
            int damage = damageAttempted(players[i], 0);
            if(isAlive(players[i]) && (damage>highestDamage || (damage==highestDamage && rand.nextDouble()>0.5)))
            {
                highestDamage = damage;
                playerToShoot = i;
            }           
        }

        if(highestDamage > 0)
        {
            System.out.print("T" + Integer.toString(playerToShoot)); // don't tell me I didn't warn you
            return;
        }

        int highestHealth = -1;
        int healthiestPlayer = -1;      
        for(int i=1; i<4; i++) // who is doing too well for their own good?
        {
            int health = getHealth(players[i]);
            if(health>highestHealth || (health==highestHealth && rand.nextDouble()>0.5))
            {
                highestHealth = health;
                healthiestPlayer = i;
            }
        }

        if(grenadeExplodes==0)
        {
            System.out.print("T" + Integer.toString(healthiestPlayer).charAt(0)); // get this hot head outta here!!
            return;
        }

        // I've got time to flaunt my grenade around

        ArrayList<Integer> playersToDodge = new ArrayList<Integer>();       
        for(int i=1; i<4; i++) // lets see who could shoot me
        {
            if(canMove(players[i]) && grenadeExplodes(players[i])!=0)
            {
                playersToDodge.add(i);
                if(grenadeExplodes(players[i])==-1) // players who have no grenade are more likely to shoot
                {
                    playersToDodge.add(i);
                }
            }
        }

        if(playersToDodge.size()>0)
        {
            System.out.print("D" + Integer.toString(playersToDodge.get(rand.nextInt(playersToDodge.size() - 1))).charAt(0)); // what do we say to would-be gunners?
            return;
        }

        if(grenadeExplodes!=1)
        {
            System.out.print("S" + Integer.toString(healthiestPlayer).charAt(0)); // seems like I can take a free shot at someone
        }
        else
        {
            System.out.print("N"); // wouldn't want to end up with an exploding grenade in my hand while being unable to throw it.
        }

    }

    public static boolean isAlive(String player) 
    {
        return player.charAt(0)!='-'; 
    }

    public static boolean canMove(String player)
    {
        return isAlive(player) && player.charAt(player.length()-2)!='S';
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

    public static int damageAttempted(String player, int target)
    {
        String[] actions = player.split(",");
        int damage = 0;
        char targetChar = Integer.toString(target).charAt(0);
        for(int i=0; i<actions.length; i++)
        {
            if(actions[i].charAt(0)=='S' && actions[i].charAt(1)==targetChar)
            {
                damage += 2;
            } 
            else if (actions[i].charAt(0)=='T')
            {
                if(actions[i].charAt(1)==targetChar)
                {
                    damage += 8;
                }
                else
                {
                    damage += 3;
                }
            }
        }

        return damage;
    }

    public static int getHealth(String player)
    {
        return Integer.parseInt(player.split(",")[0]);
    }
}
