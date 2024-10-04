-- Trigger per controllare che non ci siano intersezioni tra gli intervalli di presa in carico di ordine tra due corrieri diversi.
-- Quindi l'ordine in un istante specifico ha solo un corriere che se ne occupa
CREATE OR REPLACE FUNCTION check_no_order_overlap_intervals()
RETURNS TRIGGER AS $$
BEGIN
    IF EXISTS (
        SELECT 1
        FROM ordine_corriere o1, ordine_corriere o2
        WHERE o1.ordine = o2.ordine
        AND o1.corriere <> o2.corriere
        AND (
            (o1.inizio BETWEEN o2.inizio AND o2.fine)
            OR (o1.fine BETWEEN o2.inizio AND o2.fine)
            OR (o1.inizio <= o2.inizio AND o1.fine >= o2.fine)
        )
    ) THEN
        RAISE EXCEPTION 'Overlap between order intervals for the same courier';
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER order_overlap_intervals_trigger
BEFORE INSERT OR UPDATE ON ordine_corriere
FOR EACH ROW
EXECUTE FUNCTION check_no_order_overlap_intervals();


-- Trigger per controllare la scadenza della carta durante l'inserimento o l'aggiornamento della stessa
CREATE OR REPLACE FUNCTION check_payment_method()
RETURNS TRIGGER AS $$
BEGIN
    IF NEW.Scadenza < CURRENT_DATE THEN
        -- La carta è scaduta
        RAISE EXCEPTION 'La carta di pagamento è scaduta';
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER check_payment_method_trigger
BEFORE INSERT OR UPDATE ON Metodo_pagamento
FOR EACH ROW
EXECUTE FUNCTION check_payment_method();

-- Trigger per controllare la scadenza della carta durante la creazione di un nuovo ordine
CREATE OR REPLACE FUNCTION check_order_payment()
RETURNS TRIGGER AS $$
DECLARE
    data_pagamento DATE;
BEGIN
    SELECT Scadenza INTO data_pagamento
    FROM Metodo_pagamento
    WHERE ID = NEW.Metodo_pagamento;

    IF data_pagamento < NEW.Istante THEN
        -- Se la data di scadenza della carta è minore istante dell'ordine allora
        -- il metodo di pagamento associato all'ordine è scaduto
        RAISE EXCEPTION 'Il metodo di pagamento associato all''ordine è scaduto';
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER check_order_payment_trigger
BEFORE INSERT ON Ordine
FOR EACH ROW
EXECUTE FUNCTION check_order_payment();
