function ProgressBar({
    value = 0,
    max = 100,
    color = "linear-gradient(90deg, #6d5dfc, #4f46e5)",
    height = 8,
  }) {
    const percent = Math.min(100, Math.round((value / max) * 100));
  
    return (
      <div
        style={{
          height,
          background: "#1a2338",
          borderRadius: "999px",
          overflow: "hidden",
          width: "100%",
        }}
      >
        <div
          style={{
            width: `${percent}%`,
            height: "100%",
            background: color,
            borderRadius: "999px",
            transition: "0.4s ease",
          }}
        />
      </div>
    );
  }
  
  export default ProgressBar;