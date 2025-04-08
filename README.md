# 🛒 E-commerce Backend Project

This project is a backend system for an e-commerce platform, developed in C++ for a Bachelor's Degree course. It is designed to efficiently and securely manage interactions between clients, suppliers, and couriers with robust database management, logging, and monitoring features.

---

## 🎯 Features

- **User Management:**  
  - Registration & Login for customers, suppliers, couriers, and administrators.  
- **Product Catalog:**  
  - Suppliers manage product listings while administrators enforce product guidelines.  
- **Shopping Cart:**  
  - Customers can manage items in their shopping cart and proceed through checkout.  
- **Wishlist:**  
  - Customers can save items for future purchase.  
- **Order Management:**  
  - Customers can place or cancel orders; suppliers and couriers manage order processing.  
- **Review System:**  
  - Customers are enabled to add, view, and manage product reviews.  
- **Data Security:**  
  - Passwords are securely stored using SHA256 encryption.

---

## 🏗️ System Architecture

The backend follows an N-Tier Architecture:

1. **Application Layer:**  
   - Handles business logic and user request processing.  
   - **Technology:** C++
  
2. **Service Layer:**  
   - Manages business operations via REST API.  
   - **Technology:** RESTful services
  
3. **Data Layer:**  
   - Manages data storage and retrieval.  
   - **Technologies:**  
     - **PostgreSQL:** Relational Database  
     - **Redis:** In-memory Cache

---

## 🖥️ Operating Environment & Tools

- **Operating System:**  
  - Linux (Debian)
- **Development Tools:**  
  - Visual Studio Code, GitHub
- **Build Tools:**  
  - Makefile
- **Logging & Monitoring:**  
  - Custom-developed tools for tracking system performance
- **Security:**  
  - SHA256 encryption for managing passwords

---

## 📋 User Requirements

- **Account Management:**  
  - Users (customers, suppliers, couriers, and administrators) can register, log in, and manage their profiles.
- **Product Catalog:**  
  - Viewing, adding, and modifying products with administrative control over availability.
- **Shopping Cart & Checkout:**  
  - Manage cart items and proceed through a secure checkout process.
- **Wishlist Management:**  
  - Add, remove, and view products in a user wishlist.
- **Order Management:**  
  - Role-specific order processing for customers, suppliers, and couriers.
- **Review System:**  
  - Post, view, and delete product reviews.
- **Security & Data Protection:**  
  - Strict control over sensitive data access.
- **Performance:**  
  - Fast response times for a seamless user experience.
- **Maintainability:**  
  - Comprehensive logging to aid in diagnosing and solving issues.

---

## 📦 System Requirements

- **Web Server:**  
  - Uses Redis for web service communications.
- **Database:**  
  - PostgreSQL serves as the relational database.
- **Encryption:**  
  - SHA256 is used for secure password storage.
- **Caching:**  
  - Redis for in-memory caching to boost performance.
- **Load Balancing:**  
  - Manages request loads across different channels for various roles.
- **Logging & Monitoring:**  
  - Centralized system for tracking operations and performance.

---

## ⚙️ Implementation Details

The backend is implemented in C++ and comprises the following core components:

### HTTP Server  
- Handles incoming client requests.
- Utilizes Redis for inter-process communication.

### Database Management  
- Manages persistent data with PostgreSQL.

### Logging System  
- Records key system operations and errors for troubleshooting.

### Example Algorithms  
- **Create User:**  
  - Validates input data and manages new user registration in the database.
- **Login User:**  
  - Verifies user credentials and performs authentication.
- **Profile Management:**  
  - Retrieves and displays user profile info (e.g., payment methods, shopping cart, wishlist).
- **Order Creation:**  
  - Processes new orders while ensuring data integrity and payment validation.

---

## 📊 Experimental Results

### Logging Performance  
Three test sets were conducted to measure response times based on the number of requests:

- **1000 Logs:**  
  - Average response time: ~0.162798s
- **500 Logs:**  
  - Average response time: ~0.155826s
- **250 Logs:**  
  - Average response time: ~0.174435s

> **Note:** The logging system demonstrates stable performance with only slight fluctuations and occasional latency spikes.

---

## 🚀 How to Run

### Prerequisites

- **Redis:** Installed and running on `127.0.0.1:6379`
- **PostgreSQL:** Properly set up with required tables
- **C++ Compiler**

### Installation Steps

1. **Clone the Repository:**
    ```bash
    git clone https://github.com/your-repo/ecommerce-backend.git
    cd ecommerce-backend
    ```

2. **Build the Project:**
    ```bash
    make
    ```

---

## 📚 Libraries Used

- **stdio.h:** Standard I/O operations  
- **stdlib.h:** Memory allocation and system operations  
- **string.h:** String manipulation  
- **unistd.h:** Unix-specific functions for process management  
- **sys/wait.h:** Process management (waiting)  
- **fcntl.h:** File descriptor operations  
- **hiredis.h:** Redis client library for C

---

## 👥 Authors

- Francesco D'Aprile  
- Michelangelo Crea  
- Andrea Ladogana

---

## 📝 Conclusion

This e-commerce backend system efficiently manages the interactions between clients, suppliers, and couriers while ensuring data security and high performance. Its robust logging, monitoring, and clear architectural design make it a strong foundation for scalable and secure e-commerce solutions.

---
