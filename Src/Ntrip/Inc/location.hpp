#ifndef VRSTUNNEL_NTRIP_LOCATION_
#define VRSTUNNEL_NTRIP_LOCATION_

namespace VrsTunnel::Ntrip
{
    struct location
    {
        double Latitude;
        double Longitude;
        double Elevation;

        constexpr location() noexcept :
            Latitude { 0 },
            Longitude { 0 },
            Elevation { 0 }
            { }

        constexpr location(double latitude, double longitude, double elevation) noexcept :
            Latitude { latitude },
            Longitude { longitude },
            Elevation { elevation }
            { }
    };
    
}

#endif /* _VRSTUNNEL_NTRIP_LOCATION_ */