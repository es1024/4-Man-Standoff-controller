import java.util.List;
import java.util.ArrayList;
import java.util.Random;

class Observer {
    private static List<Integer> aggressiveEnemies = new ArrayList<>();
    private static List<Integer> enemyGrenadiers = new ArrayList<>();
    private static List<Integer> aliveEnemies = new ArrayList<>();

    public static void main(String[] args) {
        if (args[1].length() <= 7) { //first round
            Random rand = new Random();
            printResult("D" + (rand.nextInt(3) + 1));
        }
        String players[] = args[1].split(" ");

        if (truckIsOnWay(players[0])) {
            printResult("P");
        }       

        calcEnemyInfo(players);

        // end this standoff now
        if (truckWillHit(players[0])) {
            if (isGrenadier(players[0]))
                printResult("T" + aliveEnemies.get(0));
            else
                printResult("S0");
        }

        // shoot enemy who is not aggressive
        if (aggressiveEnemies.size() == 0) {
            printResult("S" + aliveEnemies.get(0));
        }

        // only one enemy to handle
        if (aggressiveEnemies.size() == 1) {
            String player = players[aggressiveEnemies.get(0)];
            if (isGrenadier(player)) {
                printResult("S" + aggressiveEnemies.get(0));
            } else if (shotLastTurn(player, aggressiveEnemies.get(0))) {
                //safe to shoot him without receiving damage
                printResult("S" + aggressiveEnemies.get(0));
            } else {
                printResult("D" + aggressiveEnemies.get(0));
            }
        }

        // multiple aggressive enemies
        if (enemyGrenadiers.size() > 0) {
            printResult("S" + enemyGrenadiers.get(0));
        } else {
            int id = aggressiveEnemies.get(0);
            for (int playerId : aggressiveEnemies) {
                if (!shotLastTurn(players[playerId], playerId)) {
                    id = playerId;
                }
            }
            printResult("D" + id);
        }
    }

    private static void printResult(String result) {
        System.out.print(result);
        System.exit(0);
    }

    private static boolean isAlive(String player) {
        return !(player.charAt(0) == '-' || player.charAt(0) == '0');
    }

    private static void calcEnemyInfo(String[] players) {
        for (int i = 1; i < players.length; i++) {
            if (isAlive(players[i])) {
                aliveEnemies.add(i);
                if (isAggressive(players[i], i)) {
                    aggressiveEnemies.add(i);
                }
                if (isGrenadier(players[i])) {
                    enemyGrenadiers.add(i);
                }
            }
        }
    }

    private static boolean truckIsOnWay(String player) {
        return player.length() - player.replace(",", "").length() == 48;
    }

    private static boolean truckWillHit(String player) {
        return player.length() - player.replace(",", "").length() == 49;
    }

    private static boolean isAggressive(String player, int id) {
        return (player.contains("S") || player.contains("P")) && !player.contains("S" + id);
    }

    private static boolean isGrenadier(String player) {
        return player.contains("P");
    }

    private static boolean shotLastTurn(String player, int id) {
        return player.charAt(player.length() - 2) == 'S' && !player.contains("S" + id);
    }
}
