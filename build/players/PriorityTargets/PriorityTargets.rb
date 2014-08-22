#!/usr/bin/env ruby

class PriorityTargets
  class PlayerAction
    SHOOT = 'S'
    DODGE = 'D'
    PREPARE_GRENADE = 'P'
    THROW_GRENADE = 'T'
    NOTHING = 'N'
    attr_accessor :action, :target

    def initialize(action_string)
        @action = action_string[0, 1]
        @target = self.has_target? ? action_string[1, 1].to_i : false
    end

    def to_s
      string = @action
      string << @target.to_s if self.has_target?
      string
    end

    def has_cooldown?
      [SHOOT].include? @action
    end

    def is_aggressive?
      [SHOOT, PREPARE_GRENADE, THROW_GRENADE].include? @action
    end

    def has_target?
      [SHOOT, DODGE, THROW_GRENADE].include? @action
    end
  end


  class Player
    attr_reader :identifier, :health, :history
    attr_accessor :playstyles

    def initialize(player_identifier, player_string)
      @identifier = player_identifier
      @playstyles = []

      player_info = player_string.split(',')
      @health = player_info.shift.to_i
      @history = parse_history(player_info)
    end


    def has_attacked?(player, round = nil)
      round ||= self.history.length - 1
      player.history[0, round].each do |turn|
        did_attack = true and break if turn.is_aggressive? && turn.has_target? && turn.target == player.identifier
      end
      did_attack ||= false
    end

    def is_holding_grenade?(round = nil)
      round ||= self.history.length
      turn_history = self.history[0, round]
      is_holding = false

      turn_history.each_with_index do |turn, curr_round|
        if turn.action == PlayerAction::PREPARE_GRENADE && curr_round >= round - 3
          is_holding = true if turn_history.drop(curr_round).select{|turn| turn.action == PlayerAction::THROW_GRENADE }.length == 0
        end
      end

      is_holding
    end

    def is_dead?; self.health <= 0; end
    def is_on_cooldown?
      return false if self.history.length == 0
      self.history.last.has_cooldown?
    end

    def turn_at_round(round); self.history[round-1]; end

    private

      def parse_history(history_array)
        parsed = []
        history_array.each {|action_string| parsed << PlayerAction.new(action_string) }
        parsed
      end
  end

  class PlayerList
    include Enumerable

    def initialize(player_list = [], filter_list = false)
      @list = player_list
      @filter = filter_list if filter_list
    end

    #Enumerable Methods
    def each
      list = @list.select{|player| @filter.include?(player.identifier) } if @filter
      list = @list unless @filter
      list.each {|player| yield(player) }
    end

    def <<(player); @list << player; end
    def [](key)
      player = @list.select{|player| @filter.include?(player.identifier) }[key] if @filter
      player = @list[key] unless @filter
      player
    end

    def length
      list = @list.select{|player| @filter.include?(player.identifier) } if @filter
      list = @list unless @filter
      list.length
    end

    def empty?; self.length == 0; end
    def not_empty?; self.length > 0; end

    def create_filtered_list(player_ids)
      new_player_list = PlayerList.new(@list, player_ids)
      new_player_list
    end

    #PlayerList Methods
    def includes_playstyle?(playstyle)
      (self.with_playstyle(playstyle).length > 0)
    end

    def have_executed_action?(action)
      action_found = false
      self.each {|player| action_found = true and break if player.history.select {|turn| turn.action == action}.length > 0 }
      action_found
    end

    def direct_damages(round = nil)
      round ||= self.first.history.length

      damage_list = {}
      @list.each {|player| damage_list[player.identifier] = 0 }

      if round >= 1
        @list.each do |player|
          player.history[0, round].each_with_index do |turn, curr_round|

            if turn.has_target?
              target_player = @list.select{|curr_player| curr_player.identifier == turn.target }.first
              target_turn = target_player.turn_at_round(curr_round)

              damage_list[turn.target] += 8 if turn.action == PlayerAction::THROW_GRENADE

              if turn.action == PlayerAction::SHOOT
                damage_list[turn.target] += 2 unless target_turn.action == PlayerAction::DODGE && target_turn.target == player.identifier
              end
            end
          end
        end
      end

      damage_list.select! {|key| @filter.include? key } if @filter
      damage_list
    end


    def filtered_with_condition(&condition_block)
      player_ids = []
      self.each {|player| player_ids << player.identifier if condition_block.call(player) }
      create_filtered_list(player_ids)
    end

    def on_cooldown; filtered_with_condition {|player| player.is_on_cooldown?} end
    def not_on_cooldown; filtered_with_condition {|player| !player.is_on_cooldown?} end

    def dead; filtered_with_condition {|player| player.is_dead?} end
    def not_dead; filtered_with_condition {|player| !player.is_dead?} end

    def with_playstyle(playstyle); filtered_with_condition {|player| player.playstyles.include?(playstyle)} end
    def not_with_playstyle(playstyle); filtered_with_condition {|player| !player.playstyles.include?(playstyle)} end

    def with_max_health(round = nil)
      round ||= self.first.history.length
      player_damages = direct_damages(round)
      filtered_with_condition {|player| player_damages[player.identifier] == player_damages.values.min }
    end

    def with_identifier(identifier)
      matches = self.with_identifiers([ identifier ])
      return nil if matches.empty?
      matches.first
    end

    def with_identifiers(identifiers)
      create_filtered_list(identifiers)
    end
  end

  class PlayerTypes
    GRENADIER = :GRENADIER
    COWBOY = :COWBOY
    SKIDDISH = :SKIDDISH
    AGGRESSOR = :AGGRESSOR
    DEFENSIVE = :DEFENSIVE
    ANTI_GRENADIER = :ANTI_GRENADIER
    PLAYSTYLE_ORDER = [GRENADIER, COWBOY, SKIDDISH, AGGRESSOR, DEFENSIVE, ANTI_GRENADIER]

    def initialize(player_list)
      @players = player_list
    end

    def analyze_playstyles
      return if @players.first.history.length == 0

      PLAYSTYLE_ORDER.each do |playstyle|
        check_fnc = "is_"+playstyle.to_s+'?'
        @players.each {|player| player.playstyles << playstyle if self.send(check_fnc, player) }
      end
    end

    def is_GRENADIER?(player)
      #Grenade on first turn
      #Used more than one grenade
      #Never used gun, only grenade
      shoot_count = player.history.count {|turn| turn.action == PlayerAction::SHOOT }
      grenade_count = player.history.count {|turn| turn.action == PlayerAction::PREPARE_GRENADE }

      profiled ||= true if player.history.first.action == PlayerAction::PREPARE_GRENADE
      profiled ||= true if grenade_count > 1
      profiled ||= true if shoot_count == 0 && grenade_count > 0
      profiled ||= false
    end

    def is_COWBOY?(player)
      #Never used grenade, only gun
      shoot_count = player.history.count {|turn| turn.action == PlayerAction::SHOOT }
      grenade_count = player.history.count {|turn| turn.action == PlayerAction::PREPARE_GRENADE }

      profiled ||= true if grenade_count == 0 && shoot_count > 0
      profiled ||= false
    end

    def is_SKIDDISH?(player)
      #Dodged more than once
      #Never hurts anybody
      dodge_count = player.history.count {|turn| turn.action == PlayerAction::DODGE }
      attack_count = player.history.count {|turn| turn.is_aggressive? }

      profiled ||= true if dodge_count > 1
      profiled ||= true if attack_count == 0 && player.history.length > 1
      profiled ||= false
    end

    def is_AGGRESSOR?(player)
      #Only shoots person >= most health
      profiled = false
      player.history.each {|turn| profiled = true if turn.is_aggressive? && turn.has_target? }

      player.history.each_with_index do |turn, round|
        if turn.is_aggressive? && turn.has_target?
          profiled = false if !@players.with_max_health(round).include? @players.with_identifier(turn.target)
        end
      end
      profiled
    end

    def is_DEFENSIVE?(player)
      #Only hurts people who hurt them first
      player.history.each {|turn| profiled = true if turn.is_aggressive? && turn.has_target? }

      player.history.each_with_index do |turn, round|
        if turn.is_aggressive? && turn.has_target?
          target_player = @players.with_identifier(turn.target)
          profiled = false unless target_player.has_attacked?(player, round)
        end
      end
      profiled ||= false
    end

    def is_ANTI_GRENADIER?(player)
      #After a Grenadier has been shown, only shoots grenadier
      shots_fired = 0
      shots_fired_while_holding = 0

      player.history.each_with_index do |turn, round|
        if turn.is_aggressive? && turn.has_target?
          target_player = @players.with_identifier(turn.target)
          shots_fired += 1
          shots_fired_while_holding += 1 if target_player.is_holding_grenade?(round)
        end
      end

      (shots_fired > 0 && shots_fired/2.0 <= shots_fired_while_holding)
    end
  end




  def initialize(game_state)
    players_info = game_state.split(' ')
    @player = Player.new(0, players_info.shift)
    @players = PlayerList.new
    @players << @player
    enemy_identifiers = []

    players_info.each_with_index {|info, index| @players << Player.new(index+1, info); enemy_identifiers << index+1; }

    @enemies = @players.with_identifiers(enemy_identifiers  )
  end

  def analyze_playstyles
    types = PlayerTypes.new(@players)
    types.analyze_playstyles
  end

  def find_dodge_target
    armed_aggressors = @enemies.with_playstyle(PlayerTypes::AGGRESSOR).not_on_cooldown().not_dead()

    if armed_aggressors.not_empty?
      return armed_aggressors.with_max_health().first if @players.with_max_health().include?(@player) && @players.with_max_health().length == 1
    end

    return @enemies[Random.rand(3)] if @player.history.length == 0
    nil
  end

  def find_target
    unarmed_aggressors = @enemies.with_playstyle(PlayerTypes::AGGRESSOR).on_cooldown().not_dead()
    anti_grenadiers = @enemies.with_playstyle(PlayerTypes::ANTI_GRENADIER).not_dead()
    grenadiers = @enemies.with_playstyle(PlayerTypes::GRENADIER).not_dead()
    cowboys = @enemies.with_playstyle(PlayerTypes::COWBOY).not_dead()
    skiddish = @enemies.with_playstyle(PlayerTypes::SKIDDISH).not_dead()
    defensive = @enemies.with_playstyle(PlayerTypes::DEFENSIVE).not_dead()

    if unarmed_aggressors.not_empty?
      return unarmed_aggressors.with_max_health().first if @players.with_max_health().include?(@player) && @players.with_max_health().length == 1
    end

    return anti_grenadiers.with_max_health().first if anti_grenadiers.not_empty?
    return grenadiers.with_max_health().first if grenadiers.not_empty?
    return cowboys.with_max_health().first if cowboys.not_empty?
    return skiddish.with_max_health().first if skiddish.not_empty?
    return defensive.with_max_health().first if defensive.not_empty?
    return @enemies.with_max_health().not_dead().first if @enemies.with_max_health().not_dead().length > 0
    nil
  end

  def find_weapon
    return PlayerAction::THROW_GRENADE if @player.is_holding_grenade?

    anti_grenadiers = @enemies.with_playstyle(PlayerTypes::ANTI_GRENADIER).not_dead()

    return PlayerAction::PREPARE_GRENADE if anti_grenadiers.empty? && @enemies.have_executed_action?(PlayerAction::PREPARE_GRENADE)
    PlayerAction::SHOOT
  end

  def make_decision
    dodge_target = self.find_dodge_target
    target = self.find_target
    weapon = self.find_weapon

    decision ||= PlayerAction.new(PlayerAction::NOTHING) if @player.is_on_cooldown? || @enemies.with_max_health().not_dead().length == 0
    decision ||= PlayerAction.new(PlayerAction::DODGE + dodge_target.identifier.to_s) if dodge_target
    decision ||= PlayerAction.new(weapon + target.identifier.to_s)
    STDOUT.write decision.to_s
  end
end

priority_targets = PriorityTargets.new(ARGV[1])
priority_targets.analyze_playstyles
priority_targets.make_decision
