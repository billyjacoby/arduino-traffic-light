patterns = [
  {
    name: "Traffic Light",
    slug: "trafficLight",
    id: 1,
    continuous: false,
    pattern: [
      {
        position: 0,
        duration: 2000,
        state: {
          red: false,
          yellow: false,
          green: true,
        },
      },
      {
        position: 1,
        duration: 500,
        state: {
          red: false,
          yellow: true,
          green: false,
        },
      },
      {
        position: 2,
        duration: 2000,
        state: {
          red: true,
          yellow: false,
          green: false,
        },
      },
    ],
  },
];
