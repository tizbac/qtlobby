
function Sqads(battleHost) {
    this.commands = [
            {name: "help", admin: false, description: "Prints this help", argc: 0, opt: 0, callback: this.cmdHelp},
            {name: "kick", admin: true, description: "Kicks a user", argc: 1, opt: 0, callback: this.cmdKick},
            {name: "force", admin: true, description: "Forces user to specified team or ally or to spectate", argc: 3, opt: 1, callback: this.cmdForce},
            {name: "start", admin: true, description: "Starts a game", argc: 0, opt: 0, callback: this.cmdStart},
            {name: "bset", admin: true, description: "Sets some battle parameter", argc: 2, opt: 0, callback: this.cmdBSet},
            {name: "map", admin: true, description: "Change current map", argc: 1, opt: 0, callback: this.cmdMap}
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
}

Sqads.prototype.onUserJoined = function(user) {
    this.bh.sayBattleEx("* Hi, " + user + "! You have joined SQADS integrated autohost. For help type !help.");
    this.users.at(user).admin = false;
};

Sqads.prototype.onUserLeft = function(user) {
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
        this.bh.sayBattleEx("* Developers probably forgot to set current username on BattleHost.");
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
    var u = this.bh.users.at(username);
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
            this.bh.sayBattleEx("* " + user + " has been kicked by " + caller);
            u.kick();
        }
    } else {
        this.bh.sayBattleEx("* No such user.");
    }
};

Sqads.prototype.cmdForce = function(caller, user, param, value) {
    var u = this.bh.users.at(user);
    switch(param) {
            case "team":
        u.team = value - 0;
        break;
            case "ally":
        u.ally = value - 0;
        break;
            case "spec":
        u.player = false;
        break;
    }
};

Sqads.prototype.cmdStart = function(caller) {
    this.bh.sayBattleEx("* Starting game...");
    this.bh.users.at(caller).ingame = true;
};

Sqads.prototype.cmdBSet = function(caller, setting, value) {
    if(this.bh.isScriptTagValueValid(setting, value)) {
        this.bh.setScriptTag(setting, value);
    } else {
        this.bh.sayBattleEx("* Value '" + value + "'is invalid for this setting");
    }
};

Sqads.prototype.cmdMap = function(caller, map) {
    if(this.bh.hasMap(map)) {
        this.bh.setMap(map);
    } else {
        this.bh.sayBattleEx("* No such map.");
    }
};
