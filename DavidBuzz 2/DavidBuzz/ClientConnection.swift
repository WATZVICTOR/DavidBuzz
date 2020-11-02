//
//  ClientConnection.swift
//  Cliente
//
//  Created by Fernando on 30/09/2020.
//  Copyright © 2020 Victor. All rights reserved.
//

import Foundation
import Network
import AVFoundation

@available(macOS 10.14, *)
class ClientConnection {
    var audio: AVAudioPlayer?;

    let  nwConnection: NWConnection
    let queue = DispatchQueue(label: "Client connection Q")

    init(nwConnection: NWConnection) {
        self.nwConnection = nwConnection
    }

    var didStopCallback: ((Error?) -> Void)? = nil

    func start() {
        print("connection will start")
        nwConnection.stateUpdateHandler = stateDidChange(to:)
        setupReceive()
        nwConnection.start(queue: queue)
    }

    private func stateDidChange(to state: NWConnection.State) {
        switch state {
        case .waiting(let error):
            connectionDidFail(error: error)
        case .ready:
            print("Client connection ready")
        case .failed(let error):
            connectionDidFail(error: error)
        default:
            break
        }
    }

    private func setupReceive() {
        nwConnection.receive(minimumIncompleteLength: 1, maximumLength: 65536) { (data, _, isComplete, error) in
            if let data = data, !data.isEmpty {
                let message = String(data: data, encoding: .utf8)
                print("connection did receive, data: \(data as NSData) string: \(message ?? "-" )\n")
                self.playAudio(message: message!)
            }
            if isComplete {
                self.connectionDidEnd()
            } else if let error = error {
                self.connectionDidFail(error: error)
            } else {
                self.setupReceive()
            }
        }
    }

    func send(data: Data) {
        nwConnection.send(content: data, completion: .contentProcessed( { error in
            if let error = error {
                self.connectionDidFail(error: error)
                return
            }
                print("connection did send, data: \(data as NSData)")
            
        }))
    }

    func stop() {
        print("connection will stop")
        stop(error: nil)
    }

    private func connectionDidFail(error: Error) {
        print("connection did fail, error: \(error)")
        self.stop(error: error)
    }

    private func connectionDidEnd() {
        print("connection did end")
        self.stop(error: nil)
    }

    private func stop(error: Error?) {
        self.nwConnection.stateUpdateHandler = nil
        self.nwConnection.cancel()
        if let didStopCallback = self.didStopCallback {
            self.didStopCallback = nil
            didStopCallback(error)
        }
    }
    
    private func playAudio(message: String) {
        print("\(message)\n");
        switch message {
        case "1":
            let path = Bundle.main.path(forResource: "1", ofType: "mp3")
            let url = URL(fileURLWithPath: path!)
            do {
                self.audio = try AVAudioPlayer(contentsOf: url)
                self.audio?.play();
            } catch {
                
            }
            break;
        case "3":
            let path = Bundle.main.path(forResource: "3", ofType: "mp3")
            let url = URL(fileURLWithPath: path!)
            do {
                self.audio = try AVAudioPlayer(contentsOf: url)
                self.audio?.play();
            } catch {
                
            }
            break;
        case "7":
            let path = Bundle.main.path(forResource: "7", ofType: "mp3")
            let url = URL(fileURLWithPath: path!)
            do {
                self.audio = try AVAudioPlayer(contentsOf: url)
                self.audio?.play();
            } catch {
                
            }
            break;
        case "9":
            let path = Bundle.main.path(forResource: "9", ofType: "mp3")
            let url = URL(fileURLWithPath: path!)
            do {
                self.audio = try AVAudioPlayer(contentsOf: url)
                self.audio?.play();
            } catch {
                
            }
            break;
        default:
            break;
        }
    }
}
