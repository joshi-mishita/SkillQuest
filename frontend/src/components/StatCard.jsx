function StatCard({
    title,
    value,
    color = "#6d5dfc",
  }) {
    return (
      <div className="stat-card">
        <h2 style={{ color }}>{value}</h2>
        <p>{title}</p>
      </div>
    );
  }
  
  export default StatCard;