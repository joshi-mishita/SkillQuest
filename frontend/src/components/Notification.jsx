import { useEffect } from "react";

function Notification({ message, onClose }) {
  useEffect(() => {
    const timer = setTimeout(() => {
      onClose();
    }, 2500);

    return () => clearTimeout(timer);
  }, [onClose]);

  if (!message) return null;

  return <div className="notification">{message}</div>;
}

export default Notification;