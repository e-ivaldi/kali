open Unix;

Printf.printf("Kali is starting\n");

let server_socket = socket(PF_INET, SOCK_STREAM, 0);
/**/
let server_addr = inet_addr_loopback; /*inet_addr_of_string("0.0.0.0")*/
let server_port = 8181;
let max_pending_requests = 1;

bind(server_socket, ADDR_INET(server_addr, server_port));

listen(server_socket, max_pending_requests);

while (true) {
  print_endline("Listening...");
  let (client_socket, client_addr) = accept(server_socket);

  let client_ip =
    switch (client_addr) {
    | ADDR_INET(address, port) => string_of_inet_addr(address)
    | _ => "unknown ip"
    };

  print_endline("Connection received from ip " ++ client_ip);

  let raw_request = Bytes.create(1024);

  let recv_bytes = recv(client_socket, raw_request, 0, 1024, []);

  print_endline("recv_bytes: " ++ string_of_int(recv_bytes));
  print_endline(" raw_request: " ++ Bytes.to_string(raw_request));

  let raw_response = {|HTTP/1.1 200 OK

    Body!

    |};

  let raw_response_bytes = Bytes.unsafe_of_string(raw_response);
  let sent_bytes = send(client_socket, raw_response_bytes, 0, Bytes.length(raw_response_bytes), []);

  print_endline("sent_bytes: " ++ string_of_int(sent_bytes));
  close(client_socket);
};

/*let (xxx, socket_addr) = accept(file_descr);*/
