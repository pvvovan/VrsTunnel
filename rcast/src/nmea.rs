use std::io::ErrorKind;

use crate::wgs84;

pub fn gga(buf: &[u8]) -> Result<wgs84::Location, Box<dyn std::error::Error>> {
    let buf = std::str::from_utf8(buf)?;
    for gga in buf.split("$GP") {
        let latstr = split_by(gga, ",", 2);
        let lonstr = split_by(gga, ",", 4);
        if latstr.is_none() || lonstr.is_none() {
            continue;
        }

        let latstr = latstr.unwrap();
        let lonstr = lonstr.unwrap();
        let lat = parse_degrees(latstr);
        let lon = parse_degrees(lonstr);
        if lat.is_none() || lon.is_none() {
            continue;
        }

        let lat = lat.unwrap();
        let lon = lon.unwrap();
        return Ok(wgs84::Location::new(lat, lon, 0.0));
    }

    Err(Box::new(std::io::Error::from(ErrorKind::NotFound)))
}

fn split_by<'a>(msg: &'a str, by: &str, index: usize) -> Option<&'a str> {
    msg.split(by)
        .enumerate()
        .filter(|(i, _)| *i == index)
        .map(|(_, v)| v)
        .take(1)
        .next()
}

fn parse_degrees(buf: &str) -> Option<f64> {
    let mindeg_str = split_by(buf, ".", 0);
    if mindeg_str.is_none() {
        return None;
    }

    let mindeg_str = mindeg_str.unwrap();
    let minutes_str = &buf[mindeg_str.len() - 2..];
    let deg_str = &mindeg_str[..mindeg_str.len() - 2];

    let mut deg: f64 = match deg_str.parse() {
        Ok(num) => num,
        _ => {
            return None;
        }
    };

    let minutes: f64 = match minutes_str.parse() {
        Ok(num) => num,
        _ => {
            return None;
        }
    };

    deg += minutes / 60.0;

    Some(deg)
}

#[cfg(test)]
mod tests {
    #[test]
    fn parsegga_err() {
        let loc = super::gga(b"");
        assert!(loc.is_err());
    }

    #[test]
    fn parsegga_ok() {
        let loc = super::gga(b"$GPGGA,115739.00,4158.8441367,N,09147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000*6E");
        assert!(loc.is_ok());
    }

    #[test]
    fn parsegga_distance() {
        let loc1 = super::gga(
            b"$GPGGA,115739.00,5743.68318,N,01156.38716,W,4,13,0.9,255.747,M,-32.00,M,01,0000*6E",
        )
        .unwrap();
        let loc2 = super::gga(
            b"$GPGGA,115739.00,5743.68252,N,01156.39676,W,4,13,0.9,255.747,M,-32.00,M,01,0000*6E",
        )
        .unwrap();
        approx::assert_relative_eq!(loc1.distance(&loc2), 10.0, epsilon = 0.5);
    }
}
