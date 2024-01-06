#![allow(
    clippy::wildcard_imports,
    clippy::cast_lossless,
    clippy::cast_possible_truncation,
    clippy::cast_sign_loss
)]

use std::{
    collections::HashMap,
    error::Error,
    fs::File,
    io::{Read, Write},
};

use esp::{ByteBufferExtRead, ByteReader};
use structs::PlayerLog;

mod structs;
mod visualizer;

fn filter_noise(log: &mut PlayerLog) {
    log.lerped.retain(|data| data.position.0 > 5.0 && data.position.1 > 5.0);
}

fn main() -> Result<(), Box<dyn Error>> {
    let mut args = std::env::args();
    args.next();
    let path = args.next().unwrap();

    let mut file = File::open(path)?;
    let mut vec = vec![];
    file.read_to_end(&mut vec)?;
    drop(file);

    let mut reader = ByteReader::from_bytes(&vec);
    let mut player_log = reader.read_value::<HashMap<i32, PlayerLog>>().map_err(|e| e.to_string())?;

    let player_log = if player_log.len() > 1 {
        let ids = player_log.keys().map(ToString::to_string).collect::<Vec<_>>().join(" ");

        println!("Multiple players found: {ids}");
        print!("> ");

        let mut input = String::new();
        std::io::stdin().read_line(&mut input)?;

        let player_id = input.trim().parse::<i32>()?;

        player_log.get_mut(&player_id)
    } else {
        let key = *player_log.keys().next().unwrap();
        player_log.get_mut(&key)
    }
    .unwrap();

    println!(
        "log entries: {} real, {} extrapolated, {} visual, {} skipped",
        player_log.real.len(),
        player_log.real_extrapolated.len(),
        player_log.lerped.len(),
        player_log.lerp_skipped.len()
    );

    filter_noise(player_log);
    visualizer::draw(player_log)?;

    // dump all into a text file

    let mut data = String::new();
    data += "-----Real data points-----\n";
    for (n, point) in player_log.real.iter().enumerate() {
        data += format!(
            "[{n}] {} - x: {}, y: {}, rot: {}\n",
            point.timestamp, point.position.0, point.position.1, point.rotation
        )
        .as_str();
    }

    data += "\n\n\n-----Extrapolated data points-----\n";
    for (n, (real, extp)) in player_log.real_extrapolated.iter().enumerate() {
        data += format!(
            "[{n} real] {} - x: {}, y: {}, rot: {}\n",
            real.timestamp, real.position.0, real.position.1, real.rotation
        )
        .as_str();

        data += format!(
            "[{n} extp] {} - x: {}, y: {}, rot: {}\n",
            extp.timestamp, extp.position.0, extp.position.1, extp.rotation
        )
        .as_str();
    }

    data += "\n\n\n-----Interpolated data points-----\n";
    for (n, point) in player_log.lerped.iter().enumerate() {
        data += format!(
            "[{n}] {} - x: {}, y: {}, rot: {}\n",
            point.timestamp, point.position.0, point.position.1, point.rotation
        )
        .as_str();
    }

    data += "\n\n\n-----Skipped data points-----\n";
    for (n, point) in player_log.lerp_skipped.iter().enumerate() {
        data += format!(
            "[{n}] {} - x: {}, y: {}, rot: {}\n",
            point.timestamp, point.position.0, point.position.1, point.rotation
        )
        .as_str();
    }

    data += "\n";

    let mut file = File::create("data.txt")?;
    file.write_all(data.as_bytes())?;

    Ok(())
}
