
function Sqads(battleHost) {
    this.commands = [
            {name: "help", admin: false, description: "Prints this help", argc: 0, opt: 0, callback: this.cmdHelp},
            {name: "kick", admin: true, description: "Kicks a user", argc: 1, opt: 0, callback: this.cmdKick},
            {name: "force", admin: true, description: "Forces user to specified team or ally or to spectate", argc: 3, opt: 1, callback: this.cmdForce},
            {name: "spec", admin: true, description: "Forces user to spectate", argc: 1, opt: 0, callback: this.cmdSpec},
            {name: "start", admin: true, description: "Starts a game", argc: 0, opt: 0, callback: this.cmdStart},
            {name: "bset", admin: true, description: "Sets some battle parameter", argc: 2, opt: 0, callback: this.cmdBSet},
            {name: "map", admin: true, description: "Change current map", argc: 1, opt: 0, callback: this.cmdMap},
            {name: "addbox", admin: false, description: "Add start box for ally team", argc: 5, opt: 0, callback: this.cmdAddBox},
            {name: "clearbox", admin: false, description: "Clears start box for ally team", argc: 1, opt: 1, callback: this.cmdClearBox},
            {name: "split", admin: false, description: "Splits map with start boxes", argc: 2, opt: 0, callback: this.cmdSplit},
            {name: "ring", admin: false, description: "Rings some player", argc: 1, opt: 1, callback: this.cmdRing},
            {name: "lock", admin: false, description: "Locks the room", argc: 0, opt: 0, callback: this.cmdLock},
            {name: "unlock", admin: false, description: "Unlocks the room", argc: 0, opt: 0, callback: this.cmdUnlock},
            {name: "autolock", admin: false, description: "Sets number of players which should be reached for locking", argc: 1, opt: 1, callback: this.cmdAutolock},
            {name: "teamsize", admin: false, description: "Sets team size", argc: 1, opt: 0, callback: this.cmdTeamSize},
            {name: "reload", admin: true, description: "Reloads sqads.js", argc: 0, opt: 0, callback: this.cmdReload},
            {name: "showdebugger", admin: true, description: "Shows SQADS script debugger window", argc: 0, opt: 0, callback: this.cmdShowDebugger}
    ];

    this.bh = battleHost;
    this.users = battleHost.users;
    with(battleHost) {
        userJoined.connect(this, this.onUserJoined);
        userLeft.connect(this, this.onUserLeft);
        chatCommand.connect(this, this.onChatCommand);
        clientStatusChanged.connect(this, this.onClientStatusChanged);
        stopped.connect(this, this.onServerStopped);
    }
    this.admins = [this.users.at(0).name];
    this.host = this.users.at(0).name;
    this.teamSize = 0;
    this.nbTeams = 2;
}

Sqads.prototype.getPlayersCount = function() {
    var number = 0;
    for(var i = 0; i < this.users.length; i++) {
        var user = this.users.at(i);
        if(user.player) number++;
    }
    return number;
};

Sqads.prototype.updateAutolock = function() {
    userCount = this.getPlayersCount();
    if(this.teamSize  && userCount >= this.teamSize*this.nbTeams) {
        this.bh.setLocked(true);
        return;
    }
    if(this.teamSize  && userCount < this.teamSize*this.nbTeams) {
        this.bh.setLocked(false);
        return;
    }
};

Sqads.prototype.onUserJoined = function(user) {
    this.bh.sayBattleEx("* Hi, " + user + "! You have joined SQADS integrated autohost. For help type !help.");
    this.updateAutolock();
};

Sqads.prototype.onUserLeft = function(user) {
    this.updateAutolock();
};

Sqads.prototype.isAdmin = function(user) {
    for(var i in this.admins) {
        if(this.admins[i] == user)
            return true;
    }
    return false;
};

Sqads.prototype.onChatCommand = function(command, user) {
    var u = this.users.at(user);
    //Retarded check
    if(!u.valid) {
        this.bh.sayBattleEx("* Developers probably forgot to set current username on BattleHost or SqadsUserListPrototype::at is bugged.");
        return;
    }
    var parts = command.split(" ");
    var found = false;
    var cmd = parts.shift();
    for(var i in this.commands) {
        var command = this.commands[i];
        if(command.name == cmd) {
            if(parts.length >= command.argc - command.opt && parts.length <= command.argc) {
                if(!command.admin || this.isAdmin(u.name)) {
                    parts.unshift(u.name);
                    command.callback.apply(this, parts);
                } else {
                    this.bh.sayBattleEx("* You need to be admin to execute this command");
                }
            } else {
                this.bh.sayBattleEx("* Incorrect number of argumens for '" + cmd + "'");
            }
            found = true;
            break;
        }
    }
    if(!found) {
        this.bh.sayBattleEx("* Incorrect command. Read help for the list of commands.");
    }
};

Sqads.prototype.onClientStatusChanged = function(username) {
    //var u = this.bh.users.at(username);
    this.updateAutolock();
};

Sqads.prototype.onServerStopped = function() {
    this.bh.sayBattleEx("* Server stopped");
};


/**********************************************************************************
****************************   AUTOHOST COMMANDS   ********************************
***********************************************************************************/


Sqads.prototype.cmdHelp = function(caller) {
    this.bh.sayUser(caller, "List of commands:");
    for(var i in this.commands) {
        var command = this.commands[i];
        this.bh.sayUser(caller, " !" + command.name + " - " + command.description);
    }
};

Sqads.prototype.cmdKick = function(caller, user) {
    var u = this.bh.users.at(user);
    if(u.valid) {
        if(u.name == this.host) {
            this.bh.sayBattleEx("* " + user + " is hosting current battle. Nice try (:");
        } else {
            u.kick();
        }
    } else {
        this.bh.sayBattleEx("* No such player.");
    }
};

Sqads.prototype.cmdForce = function(caller, user, param, value) {
    var u = this.bh.users.at(user);
    if(u.valid) {
        switch(param) {
        case "team": {
                if(value < 1 || value > 16) {
                    this.bh.sayBattleEx("* Team number must belong [1,16]");
                    break;
                }
                u.team = value - 0;
                break;
            }
        case "ally": {
                if(value < 1 || value > 16) {
                    this.bh.sayBattleEx("* Ally team number must belong [1,16]");
                    break;
                }
                u.ally = value - 0;
                break;
            }
        case "color": {
                u.color = value;
                break;
            }
        case "spec": {
                u.player = false;
                break;
            }
        }
    } else {
        this.bh.sayBattleEx("* No such player.");
    }
};

Sqads.prototype.cmdSpec = function(caller, user) {
    var u = this.bh.users.at(user);
    if(u.valid) {
        u.player = false;
    } else {
        this.bh.sayBattleEx("* No such player.");
    }
};

Sqads.prototype.cmdStart = function(caller) {
    var unready = new Array();
    for(var x = 0; x < this.users.length; x++) {
        var u = this.users.at(x);
        if(!u.ready) {
            unready.push(u.name);
        }
    }
    if(unready.length > 0) {
        this.bh.sayBattleEx("* Unable to start game. These players are not ready: " + unready.join(", "));
    } else {
        this.bh.sayBattleEx("* Starting game...");
        this.bh.users.at(caller).ingame = true;
    }
};

Sqads.prototype.cmdBSet = function(caller, setting, value) {
    if(this.bh.isScriptTagValueValid(setting, value)) {
        this.bh.setScriptTag(setting, value);
    } else {
        this.bh.sayBattleEx("* Value '" + value + "' is invalid for this setting");
    }
};

Sqads.prototype.cmdMap = function(caller, map) {
    var fullMapName = this.bh.findMap(map);
    if(fullMapName) {
        this.bh.setMap(fullMapName);
    } else {
        this.bh.sayBattleEx("* No such map.");
    }
};

Sqads.prototype.cmdAddBox = function(caller, left, top, right, bottom, ally) {
    if(left >= 0 && left <= 200 &&
       top >= 0 && top <= 200 &&
       right >= 0 && right <= 200 &&
       bottom >= 0 && bottom <= 200) {
        if(left < right && top < bottom) {
            if(ally >= 1 && ally <= 16) {
                this.bh.addStartRect(left, top, right, bottom, ally);
            } else {
                this.bh.sayBattleEx("* Wrong ally team number. Current protocol supports team number from 1 to 16");
            }
        } else {
            this.bh.sayBattleEx("* Given coordinates form a singular start box");
        }
    } else {
        this.bh.sayBattleEx("* Bad coordinates. Box coordinates must belong to [0;200]");
    }
};

Sqads.prototype.cmdClearBox = function(caller, ally) {
    if(ally) {
        if(ally >= 1 && ally <= 16) {
            this.bh.removeStartRect(ally);
        } else {
            this.bh.sayBattleEx("* Wrong ally team number. Current protocol supports team number from 1 to 16");
        }
    } else {
        this.bh.clearStartRects();
    }
};

Sqads.prototype.cmdSplit = function(caller, mode, percent) {
    this.bh.clearStartRects();
    if(percent > 0 && percent <= 100) {
        percent = percent / 100;
        switch(mode) {
        case "h": { //splits map horizontally
                this.bh.addStartRect(0, 0, 200, 200*percent, 0);
                this.bh.addStartRect(0, 200 - 200*percent, 200, 200, 1);
                break;
            }
        case "v" : {//splits map vertically
                this.bh.addStartRect(0, 0, 200*percent, 200, 0);
                this.bh.addStartRect(200 - 200*percent, 0, 200, 200, 1);
                break;
            }
        case "c1": { //splits map on top left and bottom right corners
                this.bh.addStartRect(0, 0, 200*percent, 200*percent, 0);
                this.bh.addStartRect(200 - 200*percent, 200 - 200*percent, 200, 200, 1);
                break;
            }
        case "c2" : { //splits map on top right and bottom left corners
                this.bh.addStartRect(200 - 200*percent, 0, 200, 200*percent, 0);
                this.bh.addStartRect(0, 200 - 200*percent, 200*percent, 200, 1);
                break;
            }
        case "c": { //splits map on 4 corners
                this.bh.addStartRect(0, 0, 200*percent, 200*percent, 0);
                this.bh.addStartRect(200 - 200*percent, 200 - 200*percent, 200, 200, 1);
                this.bh.addStartRect(200 - 200*percent, 0, 200, 200*percent, 2);
                this.bh.addStartRect(0, 200 - 200*percent, 200*percent, 200, 3);
                break;
            }
        case "s": { //splits map on 4 sides
                this.bh.addStartRect(100-200*percent/2,0,100+200*percent/2,200*percent, 0);
                this.bh.addStartRect(100-200*percent/2,200-200*percent,100+200*percent/2,200, 1);
                this.bh.addStartRect(0,100-200*percent/2,200*percent,100+200*percent/2, 2);
                this.bh.addStartRect(200-200*percent,100-200*percent/2,200,100+200*percent/2, 3);
                break;
            }
        }
    } else {
        this.bh.sayBattleEx("* Percent must belong to (0; 100]");
    }
};

Sqads.prototype.cmdRing = function(caller, user) {
    if(user) {
        var u = this.bh.users.at(user);
        if(u.valid) {
            u.ring();
        } else {
            this.bh.sayBattleEx("* No such user.");
        }
    } else {
        var unready = new Array();
        for(var x = 0; x < this.users.length; x++) {
            var u = this.users.at(x);
            if(!u.ready) {
                u.ring();
                unready.push(u.name);
            }
        }
        this.bh.sayBattleEx("* Ringing unready players("+unready.join(", ")+") by " + caller);
    }
};

Sqads.prototype.cmdLock = function(caller) {
    if(this.teamSize > 0) {
        this.bh.sayBattleEx("* Cannot lock battle, autoLock is enabled");
        return;
    }
    this.bh.setLocked(true);
};

Sqads.prototype.cmdUnlock = function(caller) {
    if(this.teamSize > 0) {
        this.bh.sayBattleEx("* Cannot unlock battle, autoLock is enabled");
        return;
    }
    this.bh.setLocked(false);
};

Sqads.prototype.cmdAutolock = function(caller, number) {
    if(number) {
        if(number < 0) {
            this.bh.sayBattleEx("* autolock argument must be a positive number");
            return;
        }
        this.bh.sayBattleEx("* Global setting changed by " + caller + "(teamSize=" + number + ")");
        this.teamSize = number/this.nbTeams;
        this.updateAutolock();
    } else {
        this.bh.sayBattleEx("* Autolock disabled by by " + caller);
        this.teamSize = 0;
        this.bh.setLocked(false);
    }
};

Sqads.prototype.cmdTeamSize = function(caller, number) {
    if(number < 0) {
        this.bh.sayBattleEx("* teamSize argument must be a positive number");
        return;
    }
    this.bh.sayBattleEx("* Global setting changed by " + caller + "(teamSize=" + number + ")");
    this.teamSize = number/this.nbTeams;
    this.updateAutolock();
};

Sqads.prototype.cmdReload = function(caller) {
    this.bh.reloadSqads();
};

Sqads.prototype.cmdShowDebugger = function(caller) {
    this.bh.showDebugger();
};
