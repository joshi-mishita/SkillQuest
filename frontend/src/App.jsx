import { useEffect, useState } from "react";

import Login from "./pages/Login";
import Dashboard from "./pages/Dashboard";
import Jobs from "./pages/Jobs";
import Career from "./pages/Career";
import Tracker from "./pages/Tracker";
import Skills from "./pages/Skills";

import {
  loadUser,
  saveUser,
  clearUser,
} from "./utils/storage";

function App() {
  const [user, setUser] = useState(loadUser());
  const [page, setPage] = useState("dashboard");

  useEffect(() => {
    if (user) {
      saveUser(user);
    }
  }, [user]);

  const handleAuth = (userData) => {
    setUser(userData);
    setPage("dashboard");
  };

  const handleLogout = () => {
    clearUser();
    setUser(null);
    setPage("dashboard");
  };

  if (!user) {
    return <Login onAuth={handleAuth} />;
  }

  if (page === "jobs") {
    return (
      <Jobs
        user={user}
        setPage={setPage}
        handleLogout={handleLogout}
      />
    );
  }

  if (page === "career") {
    return (
      <Career
        user={user}
        setPage={setPage}
        handleLogout={handleLogout}
      />
    );
  }

  if (page === "tracker") {
    return (
      <Tracker
        user={user}
        setPage={setPage}
        handleLogout={handleLogout}
      />
    );
  }

  if (page === "skills") {
    return (
      <Skills
        user={user}
        setPage={setPage}
        handleLogout={handleLogout}
      />
    );
  }

  return (
    <Dashboard
      user={user}
      setPage={setPage}
      onLogout={handleLogout}
    />
  );
}

export default App;