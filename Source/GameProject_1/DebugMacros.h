#pragma once

#include"DrawDebugHelpers.h"

#define DRAW_SPHERE(Location) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.0f, 24, FColor::Emerald, true);
#define DRAW_SPHERE_COLOR(Location, Color) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 8.0f, 12, Color, false, 5.0f);
#define DRAW_SPHERE_SingleFrame(Location) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.0f, 24, FColor::Emerald, false, -1.0f);

#define DRAW_LINE(StartLocation, EndLocation ) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.0f, 0, 1.5f) ;
#define DRAW_LINE_SingleFrame(StartLocation, EndLocation ) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.0f, 0, 1.5f) ;

#define DRAW_POINT(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 30.0f, FColor::Black, true);
#define DRAW_POINT_SingleFrame(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 30.0f, FColor::Black, false, -1.0f);

#define DRAW_VECTOR(StartLocation, EndLocation) if(GetWorld()) { \
 DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.0f, 0, 1.5f) ; \
 DrawDebugPoint(GetWorld(), EndLocation, 30.0f, FColor::Black, true); \
}

#define DRAW_VECTOR_SingleFrame(StartLocation, EndLocation) if(GetWorld()) { \
 DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.0f, 0, 1.5f) ; \
 DrawDebugPoint(GetWorld(), EndLocation, 30.0f, FColor::Black, false, -1.0f); \
}

#define DRAW_CROSSHAIR(StartLocation, Color)  if (GetWorld()) { \
DrawDebugCrosshairs(GetWorld(), StartLocation, 2 * Axis, 25.0f, Color, true); \
}