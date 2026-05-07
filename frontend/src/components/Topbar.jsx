function Topbar({ title, subtitle }) {
    return (
      <div className="topbar">
        <div>
          <h1>{title}</h1>
          {subtitle && <p>{subtitle}</p>}
        </div>
      </div>
    );
  }
  
  export default Topbar;