pub struct Location {
    x: f64,
    y: f64,
    z: f64,
}

impl Location {
    pub fn new(lat: f64, lon: f64, ele: f64) -> Self {
        let lat = std::f64::consts::PI * lat / 180.0;
        let lon = std::f64::consts::PI * lon / 180.0;
        let chi = (1.0 - WGS84_E_S * lat.sin().powf(2.0)).sqrt();

        Location {
            x: (WGS84_A / chi + ele) * lat.cos() * lon.cos(),
            y: (WGS84_A / chi + ele) * lat.cos() * lon.sin(),
            z: (WGS84_A / chi * (1.0 - WGS84_E_S) + ele) * lat.sin(),
        }
    }

    pub fn distance(&self, other: &Self) -> f64 {
        ((self.x - other.x).powf(2.0) + (self.y - other.y).powf(2.0) + (self.z - other.z).powf(2.0))
            .sqrt()
    }
}

const WGS84_A: f64 = 6378137.0; // earth semimajor axis in meters
const WGS84_F: f64 = 1.0 / 298.257223563; // reciprocal flattening
const WGS84_E_S: f64 = (2.0 * WGS84_F) - (WGS84_F * WGS84_F); // eccentricity squared
/// const WGS84_B: f64 = WGS84_A * (1.0 - WGS84_F); // semi-minor axis
/// const WGS84_E_S2: f64 = WGS84_F * (2.0 - WGS84_F) / ((1.0 - WGS84_F) * (1.0 - WGS84_F)); // second eccentricity squared

#[cfg(test)]
mod tests {
    use super::*;
    use approx;
    use std::f64;

    #[test]
    fn distance_1() {
        let loc1 = Location {
            x: 0.0,
            y: 0.0,
            z: 0.0,
        };
        let loc2 = Location {
            x: 1.0,
            y: 0.0,
            z: 0.0,
        };
        approx::assert_relative_eq!(loc1.distance(&loc2), 1.0, epsilon = f64::EPSILON);
    }

    #[test]
    fn distance_2() {
        let loc1 = Location {
            x: 0.0,
            y: 2.0,
            z: 0.0,
        };
        let loc2 = Location {
            x: 0.0,
            y: 0.0,
            z: 0.0,
        };
        approx::assert_relative_eq!(loc1.distance(&loc2), 2.0, epsilon = f64::EPSILON);
    }

    #[test]
    fn distance_5() {
        let loc1 = Location {
            x: 3.0,
            y: 0.0,
            z: 0.0,
        };
        let loc2 = Location {
            x: 0.0,
            y: 0.0,
            z: 4.0,
        };
        approx::assert_relative_eq!(loc1.distance(&loc2), 5.0, epsilon = f64::EPSILON);
    }

    #[test]
    fn to_cart_1() {
        let loc = Location::new(0.0, 0.0, 0.0);
        approx::assert_relative_eq!(loc.x, 6378137.0, epsilon = f64::EPSILON);
        approx::assert_relative_eq!(loc.y, 0.0, epsilon = f64::EPSILON);
        approx::assert_relative_eq!(loc.z, 0.0, epsilon = f64::EPSILON);
    }

    #[test]
    fn to_cart_2() {
        let loc = Location::new(0.0, 90.0, 0.0);
        approx::assert_relative_eq!(loc.x, 0.0, epsilon = 0.000_000_1);
        approx::assert_relative_eq!(loc.y, 6378137.0, epsilon = f64::EPSILON);
        approx::assert_relative_eq!(loc.z, 0.0, epsilon = f64::EPSILON);
    }

    #[test]
    fn to_cart_3() {
        let loc = Location::new(90.0, 0.0, 0.0);
        approx::assert_relative_eq!(loc.x, 0.0, epsilon = 0.000_000_1);
        approx::assert_relative_eq!(loc.y, 0.0, epsilon = f64::EPSILON);
        approx::assert_relative_eq!(loc.z, 6356752.314245, epsilon = 0.000_000_2);
    }
}
