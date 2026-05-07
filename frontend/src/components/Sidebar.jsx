function Sidebar({ user, onLogout, setPage }) {
    const links = [
      { id: "dashboard", label: "Dashboard" },
      { id: "jobs", label: "Jobs" },
      { id: "career", label: "Career" },
      { id: "tracker", label: "Tracker" },
      { id: "skills", label: "Skills" },
    ];
  
    return (
      <aside className="sidebar">
        <div>
          <h2 className="logo">SkillQuest</h2>
  
          <nav className="sidebar-nav">
            {links.map((link) => (
              <button
                key={link.id}
                className="nav-btn"
                onClick={() => setPage(link.id)}
              >
                {link.label}
              </button>
            ))}
          </nav>
        </div>
  
        <div className="sidebar-footer">
          <p>{user.username}</p>
  
          <button
            className="logout-btn"
            onClick={onLogout}
          >
            Logout
          </button>
        </div>
      </aside>
    );
  }
  
  export default Sidebar;