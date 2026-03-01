pub const CLIENT_PORT: u16 = 8021;
pub const SERVER_PORT: u16 = 8023;
pub const BIND_ADDR: &str = "0.0.0.0";
pub const MOUNT: &str = "DynRef";

pub struct ClientDto {
    pub name: String,
    pub hash: String,
}

pub struct ServerDto {
    pub name: String,
    pub hash: String,
    pub clients: Vec<ClientDto>,
}

pub fn load() -> Vec<ServerDto> {
    let json_str = std::fs::read_to_string("../dashb/test/DemoCfg.json").unwrap();
    let obj = json::parse(&json_str).unwrap();

    let mut dto_servers = Vec::new();
    let json_servers = &obj["Servers"];
    for i in 0..json_servers.len() {
        dto_servers.push(ServerDto {
            name: json_servers[i]["Name"].to_string(),
            hash: json_servers[i]["PasswordHash"].to_string(),
            clients: Vec::new(),
        });
    }

    let json_clients = &obj["Clients"];
    for i in 0..json_servers.len() {
        let dto_client = ClientDto {
            name: json_clients[i]["Name"].to_string(),
            hash: json_clients[i]["PasswordHash"].to_string(),
        };

        dto_servers[0].clients.push(dto_client);
    }

    dto_servers
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn json_load() {
        let servers = load();
        assert!(servers.len() == 1);
        assert!(servers[0].name == "s1");
        assert!(servers[0].hash == "h2");
        assert!(servers[0].clients.len() == 1);
        assert!(servers[0].clients[0].name == "c1");
        assert!(servers[0].clients[0].hash == "h1");
    }
}
