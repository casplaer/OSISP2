use std::io::{Read, Write};
use std::net::{TcpListener, TcpStream};
use std::process::{Command, Stdio};
use std::thread;

fn handle_client(mut stream: TcpStream) {
    let mut buffer = [0; 1024];
    loop {
        match stream.read(&mut buffer) {
            Ok(n) if n > 0 => {
                let command = String::from_utf8_lossy(&buffer[..n]);
                let output = if command.trim() == "exit" {
                    break;
                } else {
                    execute_command(&command)
                };
                if let Err(e) = stream.write_all(&output) {
                    eprintln!("Error writing to stream: {}", e);
                    break;
                }
            }
            Ok(_) | Err(_) => break,
        }
    }
}

fn execute_command(command: &str) -> Vec<u8> {
    let mut cmd = Command::new("bash");
    cmd.arg("-c").arg(command).stdin(Stdio::null()).stdout(Stdio::piped());

    if let Ok(output) = cmd.output() {
        if output.status.success() {
            output.stdout
        } else {
            output.stderr
        }
    } else {
        b"Error executing command\n".to_vec()
    }
}

fn main() {
    let listener = TcpListener::bind("127.0.0.1:8080").expect("Failed to bind");
    println!("Server listening on port 8080...");

    for stream in listener.incoming() {
        match stream {
            Ok(stream) => {
                thread::spawn(move || {
                    handle_client(stream);
                });
            }
            Err(e) => {
                eprintln!("Error accepting connection: {}", e);
            }
        }
    }
}
