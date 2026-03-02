use std::collections::*;

pub const CLIENT_PORT: u16 = 8021;
pub const SERVER_PORT: u16 = 8023;
pub const BIND_ADDR: &str = "0.0.0.0";
pub const MOUNT: &str = "DynRef";

pub struct ServerCfg {
    pub clients: HashSet<String>,
}

pub fn parse(json_str: &str) -> HashMap<String, ServerCfg> {
    let dto_servers = parse_servers(&json_str);
    let dto_clients = parse_clients(&json_str);
    let mut servers = HashMap::with_capacity(dto_servers.len());
    for dto_server in &dto_servers {
        let mut clients = HashSet::with_capacity(dto_server.client_ids.len());
        for cliend_id in &dto_server.client_ids {
            clients.insert(
                dto_clients
                    .iter()
                    .find(|c| &c.id == cliend_id)
                    .unwrap()
                    .hash
                    .clone(),
            );
        }
        servers.insert(dto_server.hash.clone(), ServerCfg { clients });
    }
    servers
}

struct ClientDto {
    _name: String,
    hash: String,
    id: String,
}

struct ServerDto {
    _name: String,
    hash: String,
    _id: String,
    client_ids: Vec<String>,
}

fn parse_clients(json_str: &str) -> Vec<ClientDto> {
    let json_obj = json::parse(&json_str).unwrap();
    let json_clients = &json_obj["Clients"];
    let mut clients = Vec::new();
    for i in 0..json_clients.len() {
        clients.push(ClientDto {
            _name: json_clients[i]["Name"].to_string(),
            hash: json_clients[i]["PasswordHash"].to_string(),
            id: json_clients[i]["Id"].to_string(),
        });
    }
    clients
}

fn parse_servers(json_str: &str) -> Vec<ServerDto> {
    let json_obj = json::parse(&json_str).unwrap();
    let json_servers = &json_obj["Servers"];
    let mut servers = Vec::with_capacity(json_servers.len());
    for i in 0..json_servers.len() {
        let client_count = json_servers[i]["Clients"].len();
        let mut client_ids = Vec::with_capacity(client_count);
        for _ in 0..client_count {
            client_ids.push(json_servers[i]["Clients"][i].to_string());
        }

        servers.push(ServerDto {
            _name: json_servers[i]["Name"].to_string(),
            hash: json_servers[i]["PasswordHash"].to_string(),
            _id: json_servers[i]["Id"].to_string(),
            client_ids,
        });
    }
    servers
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn load_cfg() {
        let servers = parse(&std::fs::read_to_string("../dashb/test/DemoCfg.json").unwrap());
        assert!(servers["czE6czE="].clients.contains("YzE6YzE="));
    }

    #[test]
    fn parse_client_one() {
        let json_str = std::fs::read_to_string("../dashb/test/DemoCfg.json").unwrap();
        let client = &parse_clients(&json_str)[0];
        assert!(client._name == "c1");
        assert!(client.hash == "YzE6YzE=");
        assert!(client.id == "d0792ba0-96cd-4c27-adcd-eacb29760402");
    }

    #[test]
    fn parse_server_one() {
        let json_str = std::fs::read_to_string("../dashb/test/DemoCfg.json").unwrap();
        let server = &parse_servers(&json_str)[0];
        assert!(server._name == "s1");
        assert!(server.hash == "czE6czE=");
        assert!(server._id == "19f9f88f-af5d-46c6-8fb5-cfa2a91824c9");
        assert!(server.client_ids[0] == "d0792ba0-96cd-4c27-adcd-eacb29760402");
    }
}
