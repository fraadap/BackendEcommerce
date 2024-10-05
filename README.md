# E-commerce Backend Project

This project is a backend system for an e-commerce platform, developed in **C++** for a Bachelor Degree course. It is designed to efficiently and securely manage the interactions between clients, suppliers, and couriers. The system includes database management, logging, and monitoring to ensure both functional and non-functional properties.

## Features

- **User Management**: Registration and login functionality for customers, suppliers, couriers, and administrators.
- **Product Catalog**: Suppliers can manage product listings, and administrators can enforce product guidelines.
- **Shopping Cart**: Customers can manage items in their shopping cart and proceed with the checkout process.
- **Wishlist**: Customers can save items to their wishlists.
- **Order Management**: Customers can place or cancel orders; suppliers and couriers can manage orders.
- **Review System**: Customers can leave and manage reviews for purchased products.
- **Data Security**: Passwords are encrypted using **SHA256** for secure storage.

## System Architecture

The backend follows a **N-Tier Architecture**:

1. **Application Layer**: Business logic and user request handling.
   - Technologies: **C++**
2. **Service Layer**: Manages business operations via **REST API**.
   - Technologies: **RESTful services**
3. **Data Layer**: Handles data management and storage.
   - Technologies: **PostgreSQL** (relational database), **Redis** (in-memory cache)

## Operating Environment

- **Operating System**: Linux (Debian)
- **Development Tools**: Visual Studio Code, GitHub
- **Build Tools**: Makefile
- **Logging & Monitoring**: Custom-developed tools
- **Security**: SHA256 encryption for password management

## User Requirements

1. **Account Management**: Users (customers, suppliers, couriers, and administrators) must be able to register, log in, and manage their accounts.
2. **Product Catalog**: Users can view, add, and modify products. Administrators can manage product availability.
3. **Shopping Cart and Checkout**: Customers can manage their cart and proceed with checkout.
4. **Wishlist**: Customers can add, remove, and view products in their wishlist.
5. **Order Management**: Customers, suppliers, and couriers can manage orders based on their roles.
6. **Review System**: Customers can post, view, and delete product reviews.
7. **Security and Data Protection**: Access to sensitive data is restricted to authorized users.
8. **Performance**: The system must provide timely responses to user interactions.
9. **Maintainability**: Logs must be available for diagnosing and solving technical issues.

## System Requirements

1. **Web Server**: Uses **Redis** for web services.
2. **Database**: **PostgreSQL** as the relational database.
3. **Encryption**: All passwords are stored using **SHA256** encryption.
4. **Caching**: **Redis** is used for in-memory caching to improve performance.
5. **Load Balancing**: Manages request loads across different channels for various roles.
6. **Logging and Monitoring**: Centralized logging system and performance monitoring.

## Implementation

The backend system is implemented in **C++** with the following core components:

- **HTTP Server**: Handles incoming client requests and forwards them to the respective managers using **Redis** for communication between processes.
- **Database Management**: Uses **PostgreSQL** for persistent data storage.
- **Logging**: Implemented to record key system operations and errors for troubleshooting.

### Example Algorithms

1. **Create User**: Handles new user creation, ensuring valid data and managing the database insertion.
2. **Login User**: Verifies credentials and authenticates the user.
3. **Profile Management**: Retrieves and displays user profile information, including payment methods, carts, and wishlists.
4. **Order Creation**: Processes new orders, ensuring valid payment methods and user information.

## Experimental Results

Tests were conducted to evaluate the system’s logging performance. Three sets of tests were performed with 1000, 500, and 250 requests. Response times were measured for each set.

### Response Time Statistics

- **For 1000 logs**: Average response time ~0.162798s.
- **For 500 logs**: Average response time ~0.155826s.
- **For 250 logs**: Average response time ~0.174435s.

### Conclusion

- The logging system performs stably across various loads, with slight fluctuations in response times.
- There is a noticeable variance between minimum and maximum response times, indicating occasional latency spikes.

## How to Run

### Prerequisites

- **Redis** installed and running on `127.0.0.1:6379`
- **PostgreSQL** setup with required tables
- **C++ Compiler**

### Installation

Clone the repository and build the project using the provided **Makefile**:

```bash
git clone https://github.com/your-repo/ecommerce-backend.git
cd ecommerce-backend
make
```

## Libraries Used
- stdio.h: Standard I/O operations
- stdlib.h: Memory allocation and system operations
- string.h: String manipulation
- unistd.h: Unix-specific functions for process management
- sys/wait.h: Process management (waiting)
- fcntl.h: File descriptor operations
- hiredis.h: Redis client library for C

## Authors 
- Francesco D'Aprile
- Michelangelo Crea
- Andrea Ladogana
